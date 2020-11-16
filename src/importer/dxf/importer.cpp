#include <importer.h>
#include <interface.h>
#include <utils.h>

#include <stack>

#include <geometry/cubicspline.h>

#include <common/exception.h>

#include <libdxfrw/libdxfrw.h>

#include <QDebug> // TODO

namespace Importer::Dxf
{

void Importer::addPolyline(const Geometry::Polyline& polyline)
{
	m_polylines.push_back(polyline);
}

Geometry::Polyline Importer::bezierToPolyline(const Geometry::Bezier &rootBezier)
{
	// Queue of bezier to convert to biarc
	std::stack<Geometry::Bezier, Geometry::Bezier::List> bezierStack({rootBezier});

	Geometry::Polyline polyline;

	while (!bezierStack.empty()) {
		const Geometry::Bezier bezier = bezierStack.top();
		bezierStack.pop();

		if (bezier.approximateLength() < m_minimumSplineLength) {
			polyline += bezier.toLine();
			continue;
		}
		else {
			const std::optional<Geometry::Biarc> optBiarc = bezier.toBiarc();
			if (optBiarc) {
				const Geometry::Biarc &biarc = *optBiarc;
				const float error = bezier.maxError(biarc);
				if (error < m_splineToArcPrecision) {
					// The approximation is close enough.
					polyline += biarc.toPolyline();
					continue;
				}
			}
		}

		// Split bezier and schedule to conversion
		const Geometry::Bezier::Pair splitted = bezier.splitHalf();
		bezierStack.push(splitted[1]);
		bezierStack.push(splitted[0]);
	}

	return polyline;
}

Importer::Importer(const std::string& filename, float splineToArcPrecision, float minimumSplineLength)
	:m_splineToArcPrecision(splineToArcPrecision),
	m_minimumSplineLength(minimumSplineLength)
{
	Interface interface(*this);

	dxfRW rw(filename.c_str());
	if (!rw.read(&interface, false)) {
		throw Common::FileException();
	}
}

Geometry::Polyline::List &&Importer::polylines()
{
	return std::move(m_polylines);
}

void Importer::convertToPolylines(const DRW_Line &line)
{
	const Geometry::Bulge bulge(toVector2D(line.basePoint), toVector2D(line.secPoint), 0.0f);

	addPolyline(Geometry::Polyline({bulge}));
}

void Importer::convertToPolylines(const DRW_Spline &spline)
{
	const bool closed = spline.flags & (1 << 0);

	Geometry::Point2DList controlPoints(spline.ncontrol);
	std::transform(spline.controllist.begin(), spline.controllist.end(),
		controlPoints.begin(), [](DRW_Coord *coord){ return toVector2D(*coord); });

	Geometry::Bezier::List beziers;
	switch (spline.degree) {
		case 2:
		{
			break;
		}
		case 3:
		{
			Geometry::CubicSpline spline(std::move(controlPoints), closed);
			beziers = spline.toBeziers();
			break;
		}
	}

	Geometry::Bezier::List convexBeziers;
	for (const Geometry::Bezier &bezier : beziers) {
		Geometry::Bezier::List splitted = bezier.splitToConvex();
		convexBeziers.insert(convexBeziers.end(), splitted.begin(), splitted.end());
	}

	// Full spline polyline
	Geometry::Polyline polyline;
	for (const Geometry::Bezier &bezier : convexBeziers) {
		polyline += bezierToPolyline(bezier);
	}

	addPolyline(polyline);
}

void Importer::convertToPolylines(const DRW_LWPolyline &lwpolyline)
{
	const int size = lwpolyline.vertlist.size();
	if (size <= 1) {
		return;
	}

	const bool opened = !(lwpolyline.flags & (1 << 0));

	// One bulge more if openeded polyline, to connect last vertex to first vertex.
	Geometry::Bulge::List bulges(size - ((int)opened));

	DRW_Vertex2D *firstVertex = lwpolyline.vertlist.front();
	DRW_Vertex2D *vertex = firstVertex;
	QVector2D start(vertex->x, vertex->y);

	for (int i = 1; i < size; ++i) {
		DRW_Vertex2D *nextVertex = lwpolyline.vertlist[i];
		const QVector2D end(nextVertex->x, nextVertex->y);

		bulges[i - 1] = Geometry::Bulge(start, end, vertex->bulge);

		// Pass to next vertex begin
		vertex = nextVertex;
		start = end;
	}

	// Create end to start bulge if closed polyline.
	if (!opened) {
		const QVector2D end(firstVertex->x, firstVertex->y);
		bulges.back() = Geometry::Bulge(start, end, 0.0f);
	}

	addPolyline(Geometry::Polyline(std::move(bulges)));
}

void Importer::convertToPolylines(const DRW_Circle &circle)
{
	const float radius = circle.radious;
	const QVector2D center(toVector2D(circle.basePoint));

	const QVector2D startPoint(center.x() - radius, center.y());
	const QVector2D endPoint(center.x() + radius, center.y());

	const Geometry::Bulge b1(startPoint, endPoint, 1.0f);
	const Geometry::Bulge b2(endPoint, startPoint, 1.0f);

	addPolyline(Geometry::Polyline({b1, b2}));
}

void Importer::convertToPolylines(const DRW_Arc &arc)
{
	const float startAngle = arc.staangle;
	const float endAngle = arc.endangle;
	const float radius = arc.radious;
	const QVector2D center(toVector2D(arc.basePoint));

	const QVector2D relativeStart = QVector2D(std::cos(startAngle), std::sin(startAngle)) * radius;
	const QVector2D relativeEnd = QVector2D(std::cos(endAngle), std::sin(endAngle)) * radius;

	const QVector2D start = relativeStart + center;
	const QVector2D end = relativeEnd + center;

	const float theta = Geometry::DeltaAngle(startAngle, endAngle);

	// Dxf arcs are CCW
	assert(theta > 0.0f);

	// Split arc in two to avoid |tangent| > 1
	if (theta > M_PI) {
		const float newtheta = theta / 2.0f;
		const float theta4 = newtheta / 4.0f;
		const float tangent = std::tan(theta4);

		const float middleAngle = startAngle + newtheta;
		const QVector2D relativeMiddle = QVector2D(std::cos(middleAngle), std::sin(middleAngle)) * radius;
		const QVector2D middle = relativeMiddle + center;

		const Geometry::Bulge bulge1(start, middle, tangent);
		const Geometry::Bulge bulge2(middle, end, tangent);

		addPolyline(Geometry::Polyline({bulge1, bulge2}));
	}
	else {
		const float theta4 = theta / 4.0f;
		const float tangent = std::tan(theta4);

		const Geometry::Bulge bulge(start, end, tangent);

		addPolyline(Geometry::Polyline({bulge}));
	}
}

}

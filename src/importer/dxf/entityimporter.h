#pragma once

#include <importer/dxf/utils.h>

#include <importer/dxf/layer.h>
#include <geometry/cubicspline.h>
#include <geometry/quadraticspline.h>

#include <libdxfrw/drw_entities.h>

#include <fmt/format.h>

namespace importer::dxf
{

class BaseEntityImporter
{
public:
	struct Settings
	{
		const double splineToArcPrecision;
		const double minimumSplineLength;
	};

protected:
	Layer &m_layer;
	const Settings &m_settings;

	void addPolyline(const geometry::Polyline &polyline);

public:
	explicit BaseEntityImporter(Layer &layer, const Settings &settings);
};

template <typename Entity>
class EntityImporter : public BaseEntityImporter
{
public:
	using BaseEntityImporter::BaseEntityImporter;

	void operator()(const Entity &entity);
};

template <>
inline void EntityImporter<DRW_Point>::operator()(const DRW_Point &point)
{
	const Eigen::Vector2d pos(toVector2D(point.basePoint));
	const geometry::Bulge bulge(pos, pos, 0.0);

	addPolyline(geometry::Polyline({bulge}));
}

template <>
inline void EntityImporter<DRW_Line>::operator()(const DRW_Line &line)
{
	const geometry::Bulge bulge(toVector2D(line.basePoint), toVector2D(line.secPoint), 0.0);

	addPolyline(geometry::Polyline({bulge}));
}

template <>
inline void EntityImporter<DRW_LWPolyline>::operator()(const DRW_LWPolyline &lwpolyline)
{
	const int size = lwpolyline.vertlist.size();
	if (size <= 1) {
		return;
	}

	const bool opened = !(lwpolyline.flags & (1 << 0));

	// One bulge more if openeded polyline, to connect last vertex to first vertex.
	geometry::Bulge::List bulges(size - ((int)opened));

	const std::shared_ptr<DRW_Vertex2D>& firstVertex = lwpolyline.vertlist.front();
	std::shared_ptr<DRW_Vertex2D> vertex = firstVertex;
	Eigen::Vector2d start(vertex->x, vertex->y);

	for (int i = 1; i < size; ++i) {
		const std::shared_ptr<DRW_Vertex2D>& nextVertex = lwpolyline.vertlist[i];
		const Eigen::Vector2d end(nextVertex->x, nextVertex->y);

		bulges[i - 1] = geometry::Bulge(start, end, vertex->bulge);

		// Pass to next vertex begin
		vertex = nextVertex;
		start = end;
	}

	// Create end to start bulge if closed polyline.
	if (!opened) {
		const Eigen::Vector2d end(firstVertex->x, firstVertex->y);
		bulges.back() = geometry::Bulge(start, end, 0.0);
	}

	addPolyline(geometry::Polyline(std::move(bulges)));
}

template <>
inline void EntityImporter<DRW_Circle>::operator()(const DRW_Circle &circle)
{
	const double radius = circle.radious;
	const Eigen::Vector2d center(toVector2D(circle.basePoint));

	const Eigen::Vector2d startPoint(center.x() - radius, center.y());
	const Eigen::Vector2d endPoint(center.x() + radius, center.y());

	const geometry::Bulge b1(startPoint, endPoint, 1.0);
	const geometry::Bulge b2(endPoint, startPoint, 1.0);

	addPolyline(geometry::Polyline({b1, b2}));
}

template <>
inline void EntityImporter<DRW_Arc>::operator()(const DRW_Arc &arc)
{
	const double radius = arc.radious;

	if (radius > 0.0) {
		const double startAngle = arc.staangle;
		const double endAngle = arc.endangle;
		const Eigen::Vector2d center(toVector2D(arc.basePoint));

		// TODO big matrix
		const Eigen::Array2d angles(startAngle, endAngle);
		const Eigen::Array2d coses = Eigen::cos(angles);
		const Eigen::Array2d sins = Eigen::sin(angles);
		const Eigen::Vector2d relativeStart = Eigen::Vector2d(coses.x(), sins.x()) * radius;
		const Eigen::Vector2d relativeEnd = Eigen::Vector2d(coses.y(), sins.y()) * radius;

		const Eigen::Vector2d start = relativeStart + center;
		const Eigen::Vector2d end = relativeEnd + center;

		const double theta = geometry::DeltaAngle(startAngle, endAngle);
	    
		// Dxf arcs are CCW
		assert(theta > 0.0);

		// Split arc in two to avoid |tangent| > 1
		if (theta > M_PI) {
			const double newtheta = theta / 2.0;
			const double theta4 = newtheta / 4.0;
			const double tangent = std::tan(theta4);

			const double middleAngle = startAngle + newtheta;
			const Eigen::Vector2d relativeMiddle = Eigen::Vector2d(std::cos(middleAngle), std::sin(middleAngle)) * radius;
			const Eigen::Vector2d middle = relativeMiddle + center;

			const geometry::Bulge bulge1(start, middle, tangent);
			const geometry::Bulge bulge2(middle, end, tangent);

			addPolyline(geometry::Polyline({bulge1, bulge2}));
		}
		else {
			const double theta4 = theta / 4.0;
			const double tangent = std::tan(theta4);

			const geometry::Bulge bulge(start, end, tangent);

			addPolyline(geometry::Polyline({bulge}));
		}
	}
}

inline geometry::Polyline bezierToPolyline(const geometry::Bezier &rootBezier, const BaseEntityImporter::Settings &settings)
{
	// Queue of bezier to convert to biarc
	std::stack<geometry::Bezier, geometry::Bezier::List> bezierStack({rootBezier});

	geometry::Polyline polyline;

	while (!bezierStack.empty()) {
		const geometry::Bezier bezier = bezierStack.top();
		bezierStack.pop();

		if (bezier.approximateLength() < settings.minimumSplineLength) {
			polyline += bezier.toLine();
			continue;
		}
		else {
			const std::optional<geometry::Biarc> optBiarc = bezier.toBiarc();
			if (optBiarc) {
				const geometry::Biarc &biarc = *optBiarc;
				const double error = bezier.maxError(biarc);
				if (error < settings.splineToArcPrecision) {
					// The approximation is close enough.
					polyline += biarc.toPolyline();
					continue;
				}
			}
		}

		// Split bezier and schedule to conversion
		const geometry::Bezier::Pair splitted = bezier.splitHalf();
		bezierStack.push(splitted[1]);
		bezierStack.push(splitted[0]);
	}

	return polyline;
}

template <>
inline void EntityImporter<DRW_Spline>::operator()(const DRW_Spline &spline)
{
	const bool closed = spline.flags & (1 << 0);

	geometry::Point2DList controlPoints(spline.ncontrol);
	std::transform(spline.controllist.begin(), spline.controllist.end(),
		controlPoints.begin(), [](const std::shared_ptr<DRW_Coord>& coord){ return toVector2D(*coord); });

	geometry::Bezier::List beziers;
	const int degree = spline.degree;
	switch (degree) {
		case 2:
		{
			geometry::QuadraticSpline spline(std::move(controlPoints), closed);
			beziers = spline.toBeziers();
			break;
		}
		case 3:
		{
			geometry::CubicSpline spline(std::move(controlPoints), closed);
			beziers = spline.toBeziers();
			break;
		}
		default:
		{
			throw std::logic_error(fmt::format("Conversion of {}d spline not implemented", degree));
			break;
		}
	}

	geometry::Bezier::List convexBeziers;
	for (const geometry::Bezier &bezier : beziers) {
		geometry::Bezier::List splitted = bezier.splitToConvex();
		convexBeziers.insert(convexBeziers.end(), splitted.begin(), splitted.end());
	}

	// Full spline polyline
	geometry::Polyline polyline;
	for (const geometry::Bezier &bezier : convexBeziers) {
		polyline += bezierToPolyline(bezier, m_settings);
	}

	addPolyline(polyline);
}

}

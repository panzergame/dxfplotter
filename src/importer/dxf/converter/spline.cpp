#include <line.h>
#include <utils.h>

#include <geometry/cubicspline.h>

#include <QDebug>

namespace Importer::Dxf
{

Geometry::Polyline bezierToPolyline(const Geometry::Bezier &rootBezier)
{
	// Queue of bezier to convert to biarc
	Geometry::Bezier::List m_bezierQueue = {rootBezier};

	Geometry::Biarc::List

	while (!m_bezierQueue.empty()) {
		const Geometry::Bezier &bezier = m_bezierQueue.back();

		const std::optional<Geometry::Biarc> optBiarc = bezier.toBiarc();
		if (optBiarc) {
			const Geometry::Biarc &biarc = *optBiarc;
			if (bezier.maxError(biarc) < 0.1) {
				// The approximation is close enough.
				continue;
			}
		}

		
	}
}

Geometry::Polyline::List convertToPolylines(const DRW_Spline &spline)
{
	const bool closed = spline.flags & (1 << 0);
	/*qInfo() << "new spline";
	qInfo() << "degree" << spline.degree;
	qInfo() << "nknots" << spline.nknots;
	qInfo() << "nfit" << spline.nfit;
	qInfo() << "ncontrol" << spline.ncontrol;
	qInfo() << "closed" << closed;

	for (DRW_Coord *coord : spline.controllist) {
		qInfo() << toVector2D(*coord);
	}*/

	Geometry::Point2DList controlPoints(spline.ncontrol);
	std::transform(spline.controllist.begin(), spline.controllist.end(),
		controlPoints.begin(), [](DRW_Coord *coord){ return toVector2D(*coord); });

	Geometry::Bezier::List beziers;
	switch (spline.degree) {
		case 2:
		{
// 			Geometry::QuadraticSplineToBezier converter(std::move(controlPoints));
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

	/*Geometry::Bezier::List subBeziers;
	for (const Geometry::Bezier &bezier : convexBeziers) {
		Geometry::Bezier curBezier = bezier;
		for (int i = 0; i < 2; ++i) {
			Geometry::Bezier::Pair pair = curBezier.split(1.0f / (2 - i));
			subBeziers.push_back(pair[0]);
			curBezier = pair[1];
		}
		subBeziers.push_back(curBezier);
	}*/

	for (const Geometry::Bezier &bezier : convexBeziers) {
		/*qInfo() << "......................";
		qInfo() << bezier.point1();
		qInfo() << bezier.control1();
		qInfo() << bezier.control2();
		qInfo() << bezier.point2();*/

		const std::optional<Geometry::Biarc> biarc = bezier.toBiarc();
		if (biarc) {
			polyline += biarc->toPolyline();
			const float t = bezier.findTAtPointWithTangent(biarc->middle(), biarc->tangentAtMiddle(), 0.01);
			qInfo() << "t :" << t;
		}
	}

	return {polyline};
}

};

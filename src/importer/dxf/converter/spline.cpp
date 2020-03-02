#include <line.h>
#include <utils.h>

#include <geometry/cubicspline.h>

#include <stack>

#include <QDebug>

namespace Importer::Dxf
{

Geometry::Polyline bezierToPolyline(const Geometry::Bezier &rootBezier)
{
	// Queue of bezier to convert to biarc
	std::stack<Geometry::Bezier, Geometry::Bezier::List> bezierStack({rootBezier});

	Geometry::Polyline polyline;

	while (!bezierStack.empty()) {
		const Geometry::Bezier &bezier = bezierStack.top();
		bezierStack.pop();

		const std::optional<Geometry::Biarc> optBiarc = bezier.toBiarc();
		if (optBiarc) {
			const Geometry::Biarc &biarc = *optBiarc;
			const float error = bezier.maxError(biarc);
			if (error < 0.0001) { // TODO const
				// The approximation is close enough.
				polyline += biarc.toPolyline();
				continue;
			}
		}

		// Split bezier and schedule to conversion
		const Geometry::Bezier::Pair splitted = bezier.splitHalf();
		bezierStack.push(splitted[1]);
		bezierStack.push(splitted[0]);
	}

	return polyline;
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
	for (const Geometry::Bezier &bezier : convexBeziers) {
		polyline += bezierToPolyline(bezier);
	}

	return {polyline};
}

};

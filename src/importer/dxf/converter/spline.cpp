#include <line.h>
#include <utils.h>

#include <geometry/spline/cubicsplinetobezier.h>
#include <geometry/spline/quadraticsplinetobezier.h>
#include <geometry/spline/beziertobiarc.h>

#include <QDebug>

namespace Importer::Dxf
{


Model::Polylines convertToPolylines(const DRW_Spline &spline)
{
	const bool closed = spline.flags & (1 << 0);
	qInfo() << "new spline";
	qInfo() << "degree" << spline.degree;
	qInfo() << "nknots" << spline.nknots;
	qInfo() << "nfit" << spline.nfit;
	qInfo() << "ncontrol" << spline.ncontrol;
	qInfo() << "closed" << closed;

	for (DRW_Coord *coord : spline.controllist) {
		qInfo() << toVector2D(*coord);
	}

	Geometry::Point2DList controlPoints(spline.ncontrol);
	std::transform(spline.controllist.begin(), spline.controllist.end(),
		controlPoints.begin(), [](DRW_Coord *coord){ return toVector2D(*coord); });

	Geometry::Spline::Bezier::List beziers;
	switch (spline.degree) {
		case 2:
		{
// 			Geometry::Spline::QuadraticSplineToBezier converter(std::move(controlPoints));
			break;
		}
		case 3:
		{
			Geometry::Spline::CubicSplineToBezier converter(std::move(controlPoints), closed);
			beziers = converter.beziers(); // TODO copy ?
			break;
		}
	}

	Geometry::Spline::BezierToBiarc biArcconverter(std::move(beziers));

	return {};
}

};

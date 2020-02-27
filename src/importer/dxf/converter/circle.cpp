#include <line.h>
#include <utils.h>

namespace Importer::Dxf
{

Geometry::Polyline::List convertToPolylines(const DRW_Circle &circle)
{
	const float radius = circle.radious;
	const QVector2D center(toVector2D(circle.basePoint));

	const QVector2D startPoint(center.x() - radius, center.y());
	const QVector2D endPoint(center.x() + radius, center.y());

	const Geometry::Bulge b1(startPoint, endPoint, 1.0f);
	const Geometry::Bulge b2(endPoint, startPoint, 1.0f);

	return {Geometry::Polyline({b1, b2})};
}

};

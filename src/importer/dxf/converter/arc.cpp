#include <line.h>
#include <utils.h>

#include <QDebug>

namespace Importer::Dxf
{

Geometry::Polylines convertToPolylines(const DRW_Arc &arc)
{
	const float startAngle = arc.staangle;
	const float endAngle = arc.endangle;
	const float radius = arc.radious;
	const QVector2D center(toVector2D(arc.basePoint));

	const QVector2D relativeStart = QVector2D(std::cos(startAngle), std::sin(startAngle)) * radius;
	const QVector2D relativeEnd = QVector2D(std::cos(endAngle), std::sin(endAngle)) * radius;

	const float theta = endAngle - startAngle;
	const float theta4 = theta / 4.0f;
	const float tangent = std::tan(theta4);

	const QVector2D start = relativeStart + center;
	const QVector2D end = relativeEnd + center;

	const Geometry::Bulge bulge(start, end, tangent);

	return {Geometry::Polyline({bulge})};
}

};

#include <line.h>
#include <utils.h>

namespace Importer::Dxf
{

Geometry::Polylines convertToPolylines(const DRW_Line &line)
{
	const Geometry::Bulge bulge(toVector2D(line.basePoint), toVector2D(line.secPoint), 0.0f);

	return {Geometry::Polyline({bulge})};
}

};

#include <line.h>
#include <utils.h>

namespace Importer::Dxf
{

Model::Polylines convertToPolylines(const DRW_Line &line)
{
	const Model::Bulge bulge(toVector2D(line.basePoint), toVector2D(line.secPoint), 0.0f);

	return {Model::Polyline({bulge})};
}

};

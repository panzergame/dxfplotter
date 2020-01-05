#include <line.h>
#include <utils.h>

namespace Importer::Dxf
{

Core::Polylines fromLine(const DRW_Line &line)
{
	const Core::Bulge bulge(toVector2D(line.basePoint), toVector2D(line.secPoint), 0.0f);

	return {Core::Polyline({bulge})};
}

};

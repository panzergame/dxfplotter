#pragma once

#include <libdxfrw/drw_entities.h>

#include <geometry/polyline.h>

namespace Importer::Dxf
{

Geometry::Polylines convertToPolylines(const DRW_Arc &arc);

}

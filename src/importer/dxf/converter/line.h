#pragma once

#include <libdxfrw/drw_entities.h>

#include <geometry/polyline.h>

namespace Importer::Dxf
{

Geometry::Polyline::List convertToPolylines(const DRW_Line &line);

}

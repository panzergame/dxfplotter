#pragma once

#include <libdxfrw/drw_entities.h>

#include <core/polyline.h>

namespace Importer::Dxf
{

Core::Polylines fromLine(const DRW_Line &line);

}

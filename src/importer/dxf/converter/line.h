#pragma once

#include <libdxfrw/drw_entities.h>

#include <model/polyline.h>

namespace Importer::Dxf
{

Model::Polylines fromLine(const DRW_Line &line);

}

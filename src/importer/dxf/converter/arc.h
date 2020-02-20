#pragma once

#include <libdxfrw/drw_entities.h>

#include <model/polyline.h>

namespace Importer::Dxf
{

Model::Polylines convertToPolylines(const DRW_Arc &arc);

}

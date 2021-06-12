#pragma once

#include <exporter/dxfplot/access.h>
#include <exporter/dxfplot/polyline.h>

#include <cereal/cereal.hpp>

#include <model/offsettedpath.h>

namespace Exporter::Dxfplot
{

template<>
struct Access<Model::OffsettedPath>
{
	template <class Archive>
	void operator()(Archive &archive, const Model::OffsettedPath &offsettedPath)
	{
// 		archive(cereal::make_nvp("offseted_polylines", offsettedPath.offsettedPolylines()));
// 		archive(cereal::make_nvp("direction", offsettedPath.cuttingDirection()));
	}
};

}


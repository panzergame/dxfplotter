#pragma once

#include <serializer/access.h>
#include <serializer/polyline.h>

#include <cereal/cereal.hpp>

#include <model/offsettedpath.h>

namespace Serializer
{

template<>
struct Access<Model::OffsettedPath>
{
	template <class Archive>
	void save(Archive &archive, const Model::OffsettedPath &offsettedPath) const
	{
// 		archive(cereal::make_nvp("offseted_polylines", offsettedPath.offsettedPolylines()));
// 		archive(cereal::make_nvp("direction", offsettedPath.cuttingDirection()));
	}
};

}


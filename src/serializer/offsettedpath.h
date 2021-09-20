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
	void serialize(Archive &archive, Model::OffsettedPath &offsettedPath) const
	{
		archive(cereal::make_nvp("offseted_polylines", offsettedPath.m_offsettedPolylines));
		archive(cereal::make_nvp("direction", offsettedPath.m_direction));
	}
};

}


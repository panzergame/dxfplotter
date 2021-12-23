#pragma once

#include <serializer/access.h>
#include <serializer/polyline.h>

#include <cereal/cereal.hpp>

#include <model/offsettedpath.h>

namespace serializer
{

template<>
struct Access<model::OffsettedPath>
{
	template <class Archive>
	void serialize(Archive &archive, model::OffsettedPath &offsettedPath, std::uint32_t const version) const
	{
		archive(cereal::make_nvp("polylines", offsettedPath.m_polylines));
		archive(cereal::make_nvp("direction", offsettedPath.m_direction));
	}
};

}


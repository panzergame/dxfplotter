module;

#include <cstdint>
#include <serializer/access.h>
#include <cereal/cereal.hpp>

export module serializer.offsettedpath;

import model.offsettedpath;
import serializer.polyline;

export namespace serializer
{

template<>
struct Access<model::OffsettedPath>
{
	template<class Archive>
	void serialize(Archive& archive, model::OffsettedPath& offsettedPath,
				   [[maybe_unused]] std::uint32_t const version) const
	{
		archive(cereal::make_nvp("polylines", offsettedPath.m_polylines));
		archive(cereal::make_nvp("direction", offsettedPath.m_direction));
	}
};

}

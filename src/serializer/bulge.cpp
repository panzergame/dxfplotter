module;

#include <cstdint>
#include <serializer/access.h>
#include <cereal/cereal.hpp>

export module serializer.bulge;

import geometry.bulge;
import serializer.qvector2d;

import geometry.bulge;

export namespace serializer
{

template<>
struct Access<geometry::Bulge>
{
	template<class Archive>
	void serialize(Archive& archive, geometry::Bulge& bulge, [[maybe_unused]] std::uint32_t const version) const
	{
		archive(cereal::make_nvp("start", bulge.start()));
		archive(cereal::make_nvp("end", bulge.end()));
		archive(cereal::make_nvp("tangent", bulge.tangent()));
	}
};

}

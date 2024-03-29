#pragma once

#include <serializer/access.h>
#include <serializer/qvector2d.h>

#include <cereal/cereal.hpp>

#include <geometry/bulge.h>

namespace serializer
{

template<>
struct Access<geometry::Bulge>
{
	template <class Archive>
	void serialize(Archive &archive, geometry::Bulge &bulge, [[maybe_unused]] std::uint32_t const version) const
	{
		archive(cereal::make_nvp("start", bulge.start()));
		archive(cereal::make_nvp("end", bulge.end()));
		archive(cereal::make_nvp("tangent", bulge.tangent()));
	}
};

}


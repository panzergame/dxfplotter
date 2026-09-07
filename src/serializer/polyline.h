#pragma once

#include <serializer/access.h>
#include <serializer/bulge.h>

#include <cereal/cereal.hpp>

import geometry.polyline;

namespace serializer
{

template<>
struct Access<geometry::Polyline>
{
	template<class Archive>
	void serialize(Archive& archive, geometry::Polyline& polyline, [[maybe_unused]] std::uint32_t const version) const
	{
		archive(cereal::make_nvp("bulges", polyline.m_bulges));
	}
};

}

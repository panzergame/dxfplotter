#pragma once

#include <serializer/access.h>
#include <serializer/bulge.h>

#include <cereal/cereal.hpp>

#include <geometry/polyline.h>

namespace serializer
{

template<>
struct Access<geometry::Polyline>
{
	template <class Archive>
	void serialize(Archive &archive, geometry::Polyline &polyline) const
	{
		archive(cereal::make_nvp("bulges", polyline.m_bulges));
	}
};

}


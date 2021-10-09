#pragma once

#include <serializer/access.h>
#include <serializer/bulge.h>

#include <cereal/cereal.hpp>

#include <geometry/polyline.h>

namespace Serializer
{

template<>
struct Access<Geometry::Polyline>
{
	template <class Archive>
	void serialize(Archive &archive, Geometry::Polyline &polyline) const
	{
		archive(cereal::make_nvp("bulges", polyline.m_bulges));
	}
};

}


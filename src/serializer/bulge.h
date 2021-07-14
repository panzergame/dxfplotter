#pragma once

#include <serializer/access.h>
#include <serializer/qvector2d.h>

#include <cereal/cereal.hpp>

#include <geometry/bulge.h>

namespace Serializer
{

template<>
struct Access<Geometry::Bulge>
{
	template <class Archive>
	void serialize(Archive &archive, Geometry::Bulge &bulge) const
	{
		archive(cereal::make_nvp("start", bulge.start()));
		archive(cereal::make_nvp("end", bulge.end()));
		archive(cereal::make_nvp("tangent", bulge.tangent()));
	}
};

}


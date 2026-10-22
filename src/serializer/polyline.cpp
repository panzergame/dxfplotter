module;

#include <cstdint>
#include <cereal/cereal.hpp>

export module serializer.polyline;

import geometry.polyline;
import serializer.bulge;
import serializer.access;

export namespace serializer
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

module;

#include <cstdint>
#include <cereal/cereal.hpp>
#include <cereal/types/optional.hpp>

export module serializer.toolpolyline;

import geometry.tooloperation.tooloperation;
import serializer.polyline;
import serializer.access;

export namespace serializer
{

template<>
struct Access<geometry::tooloperation::ToolPolyline>
{
	template<class Archive>
	void serialize(Archive& archive, geometry::tooloperation::ToolPolyline& toolPolyline,
				   [[maybe_unused]] std::uint32_t const version) const
	{
		archive(cereal::make_nvp("polyline", toolPolyline.polyline));
		archive(cereal::make_nvp("entry", toolPolyline.entry));
	}
};

}

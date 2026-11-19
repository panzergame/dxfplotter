module;

#include <cstdint>
#include <cereal/cereal.hpp>

export module serializer.toolpath;

import model.toolpath;
import serializer.polyline;
import serializer.access;

export namespace serializer
{

template<>
struct Access<model::ToolPath>
{
	template<class Archive>
	void serialize(Archive& archive, model::ToolPath& toolpath, [[maybe_unused]] std::uint32_t const version) const
	{
		archive(cereal::make_nvp("polylines", toolpath.m_polylines));
		archive(cereal::make_nvp("cuttingDirection", toolpath.m_cuttingDirection));
	}
};

}

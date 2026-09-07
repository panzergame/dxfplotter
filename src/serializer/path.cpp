module;

#include <cstdint>
#include <serializer/access.h>
#include <cereal/cereal.hpp>

export module serializer.path;

import model.path;
import serializer.offsettedpath;
import serializer.pathsettings;
import model.renderable;
import serializer.renderable;

export namespace serializer
{

template<>
struct Access<model::Path>
{
	template<class Archive>
	void serialize(Archive& archive, model::Path& path, [[maybe_unused]] std::uint32_t const version) const
	{
		archive(cereal::make_nvp("renderable", cereal::base_class<model::Renderable>(&path)));
		archive(cereal::make_nvp("base_polyline", path.m_basePolyline));
		archive(cereal::make_nvp("offsetted_path", path.m_offsettedPath));
		archive(cereal::make_nvp("settings", path.m_settings));
	}
};

}

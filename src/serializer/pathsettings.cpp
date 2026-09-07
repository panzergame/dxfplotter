module;

#include <cstdint>
#include <serializer/access.h>
#include <cereal/cereal.hpp>

export module serializer.pathsettings;

import model.path;
import model.pathsettings;
import serializer.polyline;

export namespace serializer
{

template<>
struct Access<model::PathSettings>
{
	template<class Archive>
	void serialize(Archive& archive, model::PathSettings& pathSettings,
				   [[maybe_unused]] std::uint32_t const version) const
	{
		archive(cereal::make_nvp("plane_feed_rate", pathSettings.m_planeFeedRate));
		archive(cereal::make_nvp("depth_feed_rate", pathSettings.m_depthFeedRate));
		archive(cereal::make_nvp("intensity", pathSettings.m_intensity));
		archive(cereal::make_nvp("depth", pathSettings.m_depth));
	}
};

}

#pragma once

#include <serializer/access.h>
#include <serializer/polyline.h>

#include <cereal/cereal.hpp>

#include <model/path.h>

namespace serializer
{

template<>
struct Access<model::PathSettings>
{
	template <class Archive>
	void serialize(Archive &archive, model::PathSettings &pathSettings, std::uint32_t const version) const
	{
		archive(cereal::make_nvp("plane_feed_rate", pathSettings.m_planeFeedRate));
		archive(cereal::make_nvp("depth_feed_rate", pathSettings.m_depthFeedRate));
		archive(cereal::make_nvp("intensity", pathSettings.m_intensity));
		archive(cereal::make_nvp("depth", pathSettings.m_depth));
	}
};

}


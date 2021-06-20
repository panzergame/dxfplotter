#pragma once

#include <exporter/dxfplot/access.h>
#include <exporter/dxfplot/polyline.h>

#include <cereal/cereal.hpp>

#include <model/path.h>

namespace Exporter::Dxfplot
{

template<>
struct Access<Model::PathSettings>
{
	template <class Archive> // TODO use serialize in common library
	void operator()(Archive &archive, const Model::PathSettings &pathSettings) const
	{
		archive(cereal::make_nvp("plane_feed_rate", pathSettings.planeFeedRate()));
		archive(cereal::make_nvp("depth_feed_rate", pathSettings.depthFeedRate()));
		archive(cereal::make_nvp("intensity", pathSettings.intensity()));
		archive(cereal::make_nvp("depth", pathSettings.depth()));
	}
};

}


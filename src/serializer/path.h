#pragma once

#include <serializer/access.h>
#include <serializer/offsettedpath.h>
#include <serializer/pathsettings.h>
#include <serializer/renderable.h>

#include <cereal/cereal.hpp>

#include <model/path.h>

namespace Serializer
{

template<>
struct Access<Model::Path>
{
	template <class Archive>
	void serialize(Archive &archive, Model::Path &path) const
	{
		archive(cereal::make_nvp("renderable", cereal::base_class<Model::Renderable>(&path)));

		archive(cereal::make_nvp("base_polyline", path.m_basePolyline));
		archive(cereal::make_nvp("offsetted_path", path.m_offsettedPath));
		archive(cereal::make_nvp("settings", path.m_settings));
	}
};

}


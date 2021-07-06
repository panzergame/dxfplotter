#pragma once

#include <exporter/dxfplot/access.h>
#include <exporter/dxfplot/offsettedpath.h>
#include <exporter/dxfplot/pathsettings.h>

#include <cereal/cereal.hpp>

#include <model/path.h>

namespace Exporter::Dxfplot
{

template<>
struct Access<Model::Path>
{
	template <class Archive>
	void save(Archive &archive, const Model::Path &path) const
	{
		archive(cereal::make_nvp("base_polyline", path.m_basePolyline));
		archive(cereal::make_nvp("offseted_path", path.m_offsettedPath));
		archive(cereal::make_nvp("settings", path.settings()));
		archive(cereal::make_nvp("visible", path.visible()));
	}
};

}


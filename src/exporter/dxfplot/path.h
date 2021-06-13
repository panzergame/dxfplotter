#pragma once

#include <exporter/dxfplot/access.h>
#include <exporter/dxfplot/offsettedpath.h>

#include <cereal/cereal.hpp>

#include <model/path.h>

namespace Exporter::Dxfplot
{

template<>
struct Access<Model::Path>
{
	template <class Archive>
	void operator()(Archive &archive, const Model::Path &path) const
	{
		archive(cereal::make_nvp("base_polyline", path.m_basePolyline));
		archive(cereal::make_nvp("offseted_path", path.m_offsettedPath));
	}
};

}


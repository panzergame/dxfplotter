#pragma once

#include <exporter/dxfplot/access.h>
#include <exporter/dxfplot/path.h>

#include <cereal/cereal.hpp>

#include <model/layer.h>

namespace Exporter::Dxfplot
{

template<>
struct Access<Model::Layer>
{
	template <class Archive>
	void operator()(Archive &archive, const Model::Layer &layer)
	{
		archive(cereal::make_nvp("children", layer.m_children));
	}
};

}


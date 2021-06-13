#pragma once

#include <exporter/dxfplot/access.h>
#include <exporter/dxfplot/layer.h>

#include <cereal/cereal.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

#include <utility>
#include <model/task.h>

namespace Exporter::Dxfplot
{

template<>
struct Access<Model::Task>
{
	template <class Archive>
	void operator()(Archive &archive, const Model::Task &task) const
	{
		archive(cereal::make_nvp("layers", task.m_layers));
	}
};

}

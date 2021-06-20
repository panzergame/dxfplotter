#pragma once

#include <exporter/dxfplot/task.h>
#include <exporter/dxfplot/access.h>

#include <cereal/cereal.hpp>

#include <model/document.h>

namespace Exporter::Dxfplot
{

template<>
struct Access<Model::Document>
{
	template <class Archive>
	void operator()(Archive &archive, const Model::Document &document) const
	{
		archive(cereal::make_nvp("task", document.task()));
		archive(cereal::make_nvp("profile_name", document.profileConfig().name()));
		archive(cereal::make_nvp("tool_name", document.toolConfig().name()));
	}
};

}


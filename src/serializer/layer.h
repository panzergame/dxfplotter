#pragma once

#include <serializer/access.h>
#include <serializer/path.h>

#include <cereal/cereal.hpp>

#include <model/layer.h>

namespace Serializer
{

template<>
struct Access<Model::Layer>
{
	template <class Archive>
	void serialize(Archive &archive, Model::Layer &layer) const
	{
		archive(cereal::make_nvp("renderable", cereal::base_class<Model::Renderable>(&layer)));

		archive(cereal::make_nvp("children", layer.m_children));
	}
};

}


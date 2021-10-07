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
	void save(Archive &archive, const Model::Layer &layer) const
	{
		archive(cereal::make_nvp("renderable", cereal::base_class<Model::Renderable>(&layer)));

		archive(cereal::make_nvp("children", layer.m_children));
	}

	template <class Archive>
	void load(Archive &archive, Model::Layer &layer)
	{
		archive(cereal::make_nvp("renderable", cereal::base_class<Model::Renderable>(&layer)));

		archive(cereal::make_nvp("children", layer.m_children));

		layer.assignSelfToChildren();
	}
};

}


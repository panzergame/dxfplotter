#pragma once

#include <serializer/access.h>
#include <serializer/path.h>

#include <cereal/cereal.hpp>

#include <model/layer.h>

namespace serializer
{

template<>
struct Access<model::Layer>
{
	template <class Archive>
	void save(Archive &archive, const model::Layer &layer) const
	{
		archive(cereal::make_nvp("renderable", cereal::base_class<model::Renderable>(&layer)));
		archive(cereal::make_nvp("children", layer.m_children));
	}

	template <class Archive>
	void load(Archive &archive, model::Layer &layer)
	{
		archive(cereal::make_nvp("renderable", cereal::base_class<model::Renderable>(&layer)));
		archive(cereal::make_nvp("children", layer.m_children));

		layer.assignSelfToChildren();
	}
};

}


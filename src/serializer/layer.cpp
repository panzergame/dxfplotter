module;

#include <cstdint>
#include <serializer/access.h>
#include <cereal/cereal.hpp>

export module serializer.layer;

import model.layer;
import model.path;
import model.renderable;
import serializer.path;

export namespace serializer
{

template<>
struct Access<model::Layer>
{
	template<class Archive>
	void save(Archive& archive, const model::Layer& layer, [[maybe_unused]] std::uint32_t const version) const
	{
		archive(cereal::make_nvp("renderable", cereal::base_class<model::Renderable>(&layer)));
		archive(cereal::make_nvp("children", layer.m_children));
	}

	template<class Archive>
	void load(Archive& archive, model::Layer& layer, [[maybe_unused]] std::uint32_t const version)
	{
		archive(cereal::make_nvp("renderable", cereal::base_class<model::Renderable>(&layer)));
		archive(cereal::make_nvp("children", layer.m_children));

		layer.assignSelfToChildren();
	}
};

}

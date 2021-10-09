#pragma once

#include <serializer/access.h>

#include <cereal/cereal.hpp>

#include <model/renderable.h>

namespace Serializer
{

template<>
struct Access<Model::Renderable>
{
	template <class Archive>
	void save(Archive &archive, const Model::Renderable &renderable) const
	{
		archive(cereal::make_nvp("name", renderable.m_name));

		const bool visible = renderable.m_visible;
		archive(cereal::make_nvp("visible", visible));
	}

	template <class Archive>
	void load(Archive &archive, Model::Renderable &renderable) const
	{
		archive(cereal::make_nvp("name", renderable.m_name));

		bool visible;
		archive(cereal::make_nvp("visible", visible));
		renderable.m_visible = visible;
	}
};

}


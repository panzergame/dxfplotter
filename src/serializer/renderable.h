#pragma once

#include <serializer/access.h>

#include <cereal/cereal.hpp>

#include <model/renderable.h>

namespace serializer
{

template<>
struct Access<model::Renderable>
{
	template <class Archive>
	void save(Archive &archive, const model::Renderable &renderable, [[maybe_unused]] std::uint32_t const version) const
	{
		archive(cereal::make_nvp("name", renderable.m_name));

		const bool visible = renderable.m_visible;
		archive(cereal::make_nvp("visible", visible));
	}

	template <class Archive>
	void load(Archive &archive, model::Renderable &renderable, [[maybe_unused]] std::uint32_t const version) const
	{
		archive(cereal::make_nvp("name", renderable.m_name));

		bool visible;
		archive(cereal::make_nvp("visible", visible));
		renderable.m_visible = visible;
	}
};

}


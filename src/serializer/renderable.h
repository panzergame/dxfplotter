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
	void serialize(Archive &archive, Model::Renderable &renderable) const
	{
		archive(cereal::make_nvp("name", renderable.m_name));
		archive(cereal::make_nvp("flags", renderable.m_flags));
	}
};

}


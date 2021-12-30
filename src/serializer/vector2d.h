#pragma once

#include <serializer/access.h>

#include <cereal/cereal.hpp>

#include <Eigen/Dense>

namespace serializer
{

template<>
struct Access<Eigen::Vector2d>
{
	template <class Archive>
	void serialize(Archive &archive, Eigen::Vector2d &point, std::uint32_t const version) const
	{
		archive(cereal::make_nvp("x", point.x()));
		archive(cereal::make_nvp("y", point.y()));
	}
};

}


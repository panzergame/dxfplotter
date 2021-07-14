#pragma once

#include <serializer/access.h>

#include <cereal/cereal.hpp>

#include <QVector2D>

namespace Serializer
{

template<>
struct Access<QVector2D>
{
	template <class Archive>
	void serialize(Archive &archive, QVector2D &point) const
	{
		archive(cereal::make_nvp("x", point.x()));
		archive(cereal::make_nvp("y", point.y()));
	}
};

}


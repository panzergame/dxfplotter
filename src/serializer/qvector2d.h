#pragma once

#include <serializer/access.h>

#include <cereal/cereal.hpp>

#include <QVector2D>

namespace serializer
{

template<>
struct Access<QVector2D>
{
	template <class Archive>
	void save(Archive &archive, const QVector2D &point) const
	{
		archive(cereal::make_nvp("x", point.x()));
		archive(cereal::make_nvp("y", point.y()));
	}

	template <class Archive>
	void load(Archive &archive, QVector2D &point) const
	{
		float x;
		archive(cereal::make_nvp("x", x));
		float y;
		archive(cereal::make_nvp("y", y));

		point.setX(x);
		point.setY(y);
	}
};

}


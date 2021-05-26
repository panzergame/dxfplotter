#pragma once

#include <exporter/dxfplot/access.h>

#include <cereal/cereal.hpp>

#include <QVector2D>

namespace Exporter::Dxfplot
{

template<>
struct Access<QVector2D>
{
	template <class Archive>
	void operator()(Archive &archive, const QVector2D &point)
	{
		archive(cereal::make_nvp("x", point.x()));
		archive(cereal::make_nvp("y", point.y()));
	}
};

}


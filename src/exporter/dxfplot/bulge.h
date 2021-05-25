#pragma once

#include <exporter/dxfplot/access.h>

#include <cereal/cereal.hpp>

#include <geometry/bulge.h>

namespace Exporter::Dxfplot
{

template<>
struct Access<Geometry::Bulge>
{
	template <class Archive>
	void operator()(Archive &archive, const Geometry::Bulge &bulge)
	{
		archive(42);
	}
};

}


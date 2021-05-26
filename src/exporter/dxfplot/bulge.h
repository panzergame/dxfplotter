#pragma once

#include <exporter/dxfplot/access.h>
#include <exporter/dxfplot/qvector2d.h>

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
		archive(cereal::make_nvp("start", bulge.start()));
		archive(cereal::make_nvp("end", bulge.end()));
		archive(cereal::make_nvp("tangent", bulge.tangent()));
	}
};

}


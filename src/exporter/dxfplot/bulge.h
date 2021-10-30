#pragma once

#include <exporter/dxfplot/access.h>
#include <exporter/dxfplot/qvector2d.h>

#include <cereal/cereal.hpp>

#include <geometry/bulge.h>

namespace exporter::dxfplot
{

template<>
struct Access<geometry::Bulge>
{
	template <class Archive>
	void save(Archive &archive, const geometry::Bulge &bulge) const
	{
		archive(cereal::make_nvp("start", bulge.start()));
		archive(cereal::make_nvp("end", bulge.end()));
		archive(cereal::make_nvp("tangent", bulge.tangent()));
	}
};

}


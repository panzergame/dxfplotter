#pragma once

#include <exporter/dxfplot/access.h>
#include <exporter/dxfplot/bulge.h>

#include <cereal/cereal.hpp>

#include <geometry/polyline.h>

namespace Exporter::Dxfplot
{

template<>
struct Access<Geometry::Polyline>
{
	template <class Archive>
	void operator()(Archive &archive, const Geometry::Polyline &polyline)
	{
		archive(cereal::make_nvp("bulges", polyline.m_bulges));
	}
};

}

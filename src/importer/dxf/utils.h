#pragma once

#include <libdxfrw/drw_base.h>

#include <Eigen/Dense>

namespace importer::dxf
{

inline Eigen::Vector2d toVector2D(const DRW_Coord &coord)
{
	return {coord.x, coord.y};
}

}

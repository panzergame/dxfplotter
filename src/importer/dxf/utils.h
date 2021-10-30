#pragma once

#include <libdxfrw/drw_base.h>

#include <QVector2D>

namespace importer::dxf
{

inline QVector2D toVector2D(const DRW_Coord &coord)
{
	return QVector2D(coord.x, coord.y);
}

}

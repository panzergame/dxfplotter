module;

#include <QVector2D>

#include <libdxfrw/drw_base.h>

export module importer.dxf.utils;

export namespace importer::dxf
{

inline QVector2D toVector2D(const DRW_Coord& coord)
{
	return QVector2D(coord.x, coord.y);
}

}

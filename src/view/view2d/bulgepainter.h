#pragma once

#include <model/path.h>

#include <QPainterPath>

namespace View::View2d
{

class BulgePainter
{
private:
	QPainterPath &m_painter;

	void lineToArcPoint(const QVector2D &center, float radius, float angle);

public:
	explicit BulgePainter(QPainterPath &painter);

	void operator()(const Geometry::Bulge &bulge);
};

}

#pragma once

#include <model/path.h>

#include <QPainterPath>

namespace view::view2d
{

class BulgePainter
{
private:
	QPainterPath &m_painter;

	void lineToArcPoint(const Eigen::Vector2d &center, float radius, float angle);

public:
	explicit BulgePainter(QPainterPath &painter);

	void operator()(const geometry::Bulge &bulge);
};

}

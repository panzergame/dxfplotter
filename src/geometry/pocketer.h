#pragma once

#include <polyline.h>

namespace geometry
{

class Pocketer
{
private:
	const Polyline& m_border;
	const Polyline::List &m_islands;
	const float m_margin;

	Polyline::List m_polylines;

public:
	explicit Pocketer(const Polyline &boder, const Polyline::List &islands, float margin);

	Polyline::List &&polylines();
};

}

#pragma once

#include <geometry/polyline.h>

namespace geometry::filter
{

/** @brief Sort polyline by length.
 */
class Sorter
{
private:
	Polyline::List m_polylines;

public:
	explicit Sorter(Polyline::List &&polylines);

	Polyline::List &&polylines();
};

}

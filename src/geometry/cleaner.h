#pragma once

#include <geometry/polyline.h>

#include <set>

namespace geometry
{

/** @brief Clean polyline via merging of small bulges.
 */
class Cleaner
{
private:
	Polyline::List m_polylines;

public:
	explicit Cleaner(Polyline::List &&polylines, float minimumPolylineLength, float minimumArcLength);

	Polyline::List &&polylines();
};

}

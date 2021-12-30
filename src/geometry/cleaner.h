#pragma once

#include <geometry/polyline.h>

namespace geometry
{

/** @brief Clean polyline via merging of small bulges.
 */
class Cleaner
{
private:
	Polyline::List m_polylines;

public:
	explicit Cleaner(Polyline::List &&polylines, double minimumPolylineLength, double minimumArcLength);

	Polyline::List &&polylines();
};

}

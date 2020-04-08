#pragma once

#include <geometry/polyline.h>

#include <set>

namespace Geometry
{

/** @brief Clean polyline via merging of small bulges.
 */
class Cleaner
{
private:
	Polyline::List m_polylines;
	const float m_mergeTolerance;

public:
	explicit Cleaner(Polyline::List &&polylines, float mergeTolerance);

	Polyline::List &&polylines();
};

}

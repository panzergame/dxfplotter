#pragma once

#include <geometry/polyline.h>

#include <common/aggregable.h>

namespace Geometry
{

class Layer : public Common::Aggregable<Layer>
{
private:
	Polyline::List m_polylines;

public:
	explicit Layer() = default;

	void addPolyline(const Polyline& polyline);

	Polyline::List &&polylines();
};

}


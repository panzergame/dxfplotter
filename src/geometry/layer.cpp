#include <layer.h>

namespace Geometry
{

void Layer::addPolyline(const Polyline& polyline)
{
	m_polylines.push_back(polyline);
}

Polyline::List &&Layer::polylines()
{
	return std::move(m_polylines);
}

}



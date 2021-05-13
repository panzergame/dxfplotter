#include <layer.h>

namespace Importer::Dxf
{

Layer::Layer(const std::string& name)
	:m_name(name)
{
}

void Layer::addPolyline(const Geometry::Polyline& polyline)
{
	m_polylines.push_back(polyline);
}

Geometry::Polyline::List &&Layer::polylines()
{
	return std::move(m_polylines);
}

const std::string& Layer::name() const
{
	return m_name;
}

}



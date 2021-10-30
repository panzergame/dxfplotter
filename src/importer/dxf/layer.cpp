#include <layer.h>

namespace importer::dxf
{

Layer::Layer(const std::string& name)
	:m_name(name)
{
}

void Layer::addPolyline(const geometry::Polyline& polyline)
{
	m_polylines.push_back(polyline);
}

geometry::Polyline::List &&Layer::polylines()
{
	return std::move(m_polylines);
}

const std::string& Layer::name() const
{
	return m_name;
}

}



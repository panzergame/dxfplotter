module;

#include <string>
#include <vector>

export module importer.dxf.layer;

import common.aggregable;
import geometry.polyline;

export namespace importer::dxf
{

class Layer : public common::Aggregable<Layer>
{
private:
	geometry::Polyline::List m_polylines;
	std::string m_name;

public:
	Layer() = default;

	explicit Layer(const std::string& name);

	void addPolyline(const geometry::Polyline& polyline);

	geometry::Polyline::List&& polylines();

	const std::string& name() const;
};

}

namespace importer::dxf
{

Layer::Layer(const std::string& name)
	: m_name(name)
{
}

void Layer::addPolyline(const geometry::Polyline& polyline)
{
	m_polylines.push_back(polyline);
}

geometry::Polyline::List&& Layer::polylines()
{
	return std::move(m_polylines);
}

const std::string& Layer::name() const
{
	return m_name;
}

}

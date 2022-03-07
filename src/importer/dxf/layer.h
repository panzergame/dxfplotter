#pragma once

#include <geometry/polyline.h>

#include <common/aggregable.h>

namespace importer::dxf
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

	geometry::Polyline::List &&polylines();

	const std::string& name() const;
};

}


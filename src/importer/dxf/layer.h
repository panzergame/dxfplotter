#pragma once

#include <geometry/polyline.h>

#include <common/aggregable.h>

namespace Importer::Dxf
{

class Layer : public Common::Aggregable<Layer>
{
private:
	Geometry::Polyline::List m_polylines;
	std::string m_name;

public:
	Layer() = default;

	explicit Layer(const std::string& name);

	void addPolyline(const Geometry::Polyline& polyline);

	Geometry::Polyline::List &&polylines();

	const std::string& name() const;
};

}


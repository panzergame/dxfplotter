#pragma once

#include <geometry/polyline.h>

#include <string>

namespace Importer::Dxf
{

class Importer
{
	friend void operator<<(Importer &importer, const Geometry::Polylines &polylines);

private:
	Geometry::Polylines m_polylines;

public:
	explicit Importer(const std::string &filename);

	Geometry::Polylines &&polylines();
};

void operator<<(Importer &importer, const Geometry::Polylines &polylines);

}

#pragma once

#include <core/polyline.h>

#include <string>

namespace Importer::Dxf
{

class Importer
{
	friend void operator<<(Importer &importer, const Core::Polylines &polylines);

private:
	Core::Polylines m_polylines;

public:
	explicit Importer(const std::string &filename);

	Core::Polylines &&polylines();
};

void operator<<(Importer &importer, const Core::Polylines &polylines);

}

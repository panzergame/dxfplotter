#pragma once

#include <core/polyline.h>

namespace Core
{

class Path
{
private:
	Polyline m_polyline;

public:
	explicit Path(Polyline &&polyline);
};

using Paths = std::vector<Path>;

}

#pragma once

#include <geometry/polyline.h>

#include <common/aggregable.h>

#include <model/renderable.h>
#include <model/pathsettings.h>

#include <string>

namespace Model
{

class Layer;

class OffsettedPath
{
public:
	enum class Direction
	{
		LEFT,
		RIGHT
	};

private:
	const Geometry::Polyline::List m_offsettedPolylines;
	const Direction m_direction;

public:
	explicit OffsettedPath(Geometry::Polyline::List &&offsettedPolylines, Direction direction);

	const Geometry::Polyline::List &offsettedPolylines() const;
	Direction direction() const;
};

}

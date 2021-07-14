#include <offsettedpath.h>

namespace Model
{

OffsettedPath::OffsettedPath(Geometry::Polyline::List &&offsettedPolylines, Direction direction)
	:m_offsettedPolylines(offsettedPolylines),
	m_direction(direction)
{
}

const Geometry::Polyline::List &OffsettedPath::offsettedPolylines() const
{
	return m_offsettedPolylines;
}

Geometry::CuttingDirection OffsettedPath::cuttingDirection() const
{
	static const Geometry::CuttingDirection offsetDirectionToCuttingDirection[] = {
		Geometry::CuttingDirection::FORWARD, // OffsettedPath::Direction::LEFT
		Geometry::CuttingDirection::BACKWARD // OffsettedPath::Direction::RIGHT
	};

	return offsetDirectionToCuttingDirection[static_cast<int>(m_direction)];
}

}

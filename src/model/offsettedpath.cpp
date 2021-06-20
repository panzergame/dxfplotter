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

OffsettedPath::Direction OffsettedPath::direction() const
{
	return m_direction;
}

}


#include <offsettedpath.h>

namespace Model
{

OffsettedPath::OffsettedPath(Geometry::Polyline::List &&offsettedPolylines, Direction direction)
	:m_polylines(offsettedPolylines),
	m_direction(direction)
{
}

const Geometry::Polyline::List &OffsettedPath::polylines() const
{
	return m_polylines;
}

Geometry::CuttingDirection OffsettedPath::cuttingDirection() const
{
	static const Geometry::CuttingDirection offsetDirectionToCuttingDirection[] = {
		Geometry::CuttingDirection::FORWARD, // OffsettedPath::Direction::LEFT
		Geometry::CuttingDirection::BACKWARD // OffsettedPath::Direction::RIGHT
	};

	return offsetDirectionToCuttingDirection[static_cast<int>(m_direction)];
}

void OffsettedPath::transform(const QTransform &matrix)
{
	for (Geometry::Polyline &polyline : m_polylines) {
		polyline.transform(matrix);
	}

	emit polylinesTransformed();
}

}

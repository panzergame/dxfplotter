#include <offsettedpath.h>

namespace model
{

OffsettedPath::OffsettedPath(geometry::Polyline::List &&offsettedPolylines, Direction direction)
	:m_polylines(offsettedPolylines),
	m_direction(direction)
{
}

const geometry::Polyline::List &OffsettedPath::polylines() const
{
	return m_polylines;
}

geometry::CuttingDirection OffsettedPath::cuttingDirection() const
{
	static const geometry::CuttingDirection offsetDirectionToCuttingDirection[] = {
		geometry::CuttingDirection::FORWARD, // OffsettedPath::Direction::LEFT
		geometry::CuttingDirection::BACKWARD // OffsettedPath::Direction::RIGHT
	};

	return offsetDirectionToCuttingDirection[static_cast<int>(m_direction)];
}

void OffsettedPath::transform(const QTransform &matrix)
{
	for (geometry::Polyline &polyline : m_polylines) {
		polyline.transform(matrix);
	}

	emit polylinesTransformed();
}

}

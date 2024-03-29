#include <line.h>

namespace geometry
{

Line::Line(const QVector2D &start, const QVector2D &end)
	:m_start(start),
	m_end(end)
{
}

const QVector2D &Line::start() const
{
	return m_start;
}

const QVector2D &Line::end() const
{
	return m_end;
}

bool Line::lengthNonZero() const
{
	return (m_start != m_end);
}

float Line::length() const
{
	return m_start.distanceToPoint(m_end);
}

}

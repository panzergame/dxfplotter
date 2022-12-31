#include <arc.h>
#include <utils.h>

namespace geometry
{

Arc::Arc(const Circle &circle, const QVector2D &start, const QVector2D &end,
			float starAngle, float endAngle)
	:Circle(circle),
	m_start(start),
	m_end(end),
	m_startAngle(starAngle),
	m_endAngle(endAngle)
{
	if (orientation() == Orientation::CCW) {
		m_endAngle = EnsureEndGreater(m_startAngle, m_endAngle);
		assert(m_startAngle <= m_endAngle);
	}
	else {
		m_startAngle = EnsureEndGreater(m_endAngle, m_startAngle);
		assert(m_endAngle <= m_startAngle);
	}

	m_spanAngle = m_endAngle - m_startAngle;
}

const QVector2D &Arc::start() const
{
	return m_start;
}

const QVector2D &Arc::end() const
{
	return m_end;
}

float Arc::startAngle() const
{
	return m_startAngle;
}

float Arc::endAngle() const
{
	return m_endAngle;
}

float Arc::spanAngle() const
{
	return m_spanAngle;
}

float Arc::length() const
{
	return std::abs(m_spanAngle) * radius();
}

QVector2D Arc::pointAtAngle(float angle) const
{
	const QVector2D relativeNormalizedPoint(std::cos(angle), std::sin(angle));
	const QVector2D point = (center() + relativeNormalizedPoint * radius());
	return point;
}

}

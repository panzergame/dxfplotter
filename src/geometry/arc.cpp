#include <arc.h>
#include <utils.h>

namespace geometry
{

Arc::Arc(const Circle &circle, const Eigen::Vector2d &start, const Eigen::Vector2d &end,
			double starAngle, double endAngle)
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

const Eigen::Vector2d &Arc::start() const
{
	return m_start;
}

const Eigen::Vector2d &Arc::end() const
{
	return m_end;
}

double Arc::startAngle() const
{
	return m_startAngle;
}

double Arc::endAngle() const
{
	return m_endAngle;
}

double Arc::spanAngle() const
{
	return m_spanAngle;
}

}

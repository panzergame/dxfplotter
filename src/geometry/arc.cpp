module;

#include <cassert>
#include <cmath>
#include <QVector2D>

export module geometry.arc;

import geometry.circle;
import geometry.utils;

export namespace geometry
{

/** Arcs are considered as angle clamped circles
 */
class Arc : public Circle
{
private:
	QVector2D m_start;
	QVector2D m_end;
	float m_startAngle;
	float m_endAngle;
	float m_spanAngle;

public:
	explicit Arc(const Circle& circle, const QVector2D& start, const QVector2D& end, float starAngle, float endAngle)
		: Circle(circle)
		, m_start(start)
		, m_end(end)
		, m_startAngle(starAngle)
		, m_endAngle(endAngle)
	{
		if (orientation() == Orientation::CCW) {
			m_endAngle = EnsureEndGreater(m_startAngle, m_endAngle);
			assert(m_startAngle <= m_endAngle);
		} else {
			m_startAngle = EnsureEndGreater(m_endAngle, m_startAngle);
			assert(m_endAngle <= m_startAngle);
		}

		m_spanAngle = m_endAngle - m_startAngle;
	}

	const QVector2D& start() const { return m_start; }

	const QVector2D& end() const { return m_end; }

	float startAngle() const { return m_startAngle; }

	float endAngle() const { return m_endAngle; }

	float spanAngle() const { return m_spanAngle; }

	float length() const { return std::abs(m_spanAngle) * radius(); }

	QVector2D pointAtAngle(float angle) const
	{
		const QVector2D relativeNormalizedPoint(std::cos(angle), std::sin(angle));
		const QVector2D point = (center() + relativeNormalizedPoint * radius());
		return point;
	}

	template<class Visitor>
	void approximateToLinesVisit(float maxError, Visitor&& visitor) const
	{
		// Calculate the angle step to not exceed allowed error (distance from line to arc).
		const float angleStep = std::fmax(std::acos(1.0f - maxError) * 2.0f, maxError);

		if (orientation() == geometry::Orientation::CCW) {
			for (float angle = m_startAngle + angleStep, end = m_endAngle; angle < end; angle += angleStep) {
				visitor(pointAtAngle(angle));
			}
		} else {
			for (float angle = m_startAngle - angleStep, end = m_endAngle; angle > end; angle -= angleStep) {
				visitor(pointAtAngle(angle));
			}
		}

		visitor(m_end);
	}
};

}

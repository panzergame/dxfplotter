#pragma once

#include <geometry/circle.h>

namespace geometry
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

	template <class Visitor>
	void lineToArcPointVisit(float angle, Visitor &visitor) const
	{
		const QVector2D relativeNormalizedPoint(std::cos(angle), std::sin(angle));
		const QVector2D point = (center() + relativeNormalizedPoint * radius());
		visitor(point);
	}

public:
	explicit Arc(const Circle &circle, const QVector2D &start, const QVector2D &end,
			float starAngle, float endAngle);

	const QVector2D &start() const;
	const QVector2D &end() const;
	float startAngle() const;
	float endAngle() const;
	float spanAngle() const;

	float length() const;

	template <class Visitor>
	void approximateToLinesVisit(float maxError, Visitor &&visitor) const
	{
		// Calculate the angle step to not exceed allowed error (distance from line to arc).
		const float angleStep = std::fmax(std::acos(1.0f - maxError) * 2.0f, maxError);

		if (orientation() == geometry::Orientation::CCW) {
			for (float angle = m_startAngle + angleStep, end = m_endAngle; angle < end; angle += angleStep) {
				lineToArcPointVisit(angle, visitor);
			}
		}
		else {
			for (float angle = m_startAngle - angleStep, end = m_endAngle; angle > end; angle -= angleStep) {
				lineToArcPointVisit(angle, visitor);
			}
		}

		visitor(m_end);
	}
};

}

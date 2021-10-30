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

public:
	explicit Arc(const Circle &circle, const QVector2D &start, const QVector2D &end,
			float starAngle, float endAngle);

	const QVector2D &start() const;
	const QVector2D &end() const;
	float startAngle() const;
	float endAngle() const;
	float spanAngle() const;
};

};

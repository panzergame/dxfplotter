#pragma once

#include <geometry/circle.h>

namespace geometry
{

/** Arcs are considered as angle clamped circles
 */
class Arc : public Circle
{
private:
	Eigen::Vector2d m_start;
	Eigen::Vector2d m_end;
	double m_startAngle;
	double m_endAngle;
	double m_spanAngle;

public:
	explicit Arc(const Circle &circle, const Eigen::Vector2d &start, const Eigen::Vector2d &end,
			double starAngle, double endAngle);

	const Eigen::Vector2d &start() const;
	const Eigen::Vector2d &end() const;
	double startAngle() const;
	double endAngle() const;
	double spanAngle() const;
};

};

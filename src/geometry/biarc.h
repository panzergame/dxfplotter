#pragma once

#include <QVector2D>

#include <common/aggregable.h>

#include <geometry/utils.h>
#include <geometry/polyline.h>

namespace geometry
{

class Biarc : public Common::Aggregable<Biarc>
{
private:
	Eigen::Vector2d m_point1;
	Eigen::Vector2d m_point2;
	/// Intersection point of the arcs
	Eigen::Vector2d m_middle;
	/// Tangent at point1
	Eigen::Vector2d m_tangent1;
	/// Tangent at point1
	Eigen::Vector2d m_tangent2;
	/// Line from point1 to middle
	Eigen::Vector2d m_line1;
	/// Line from point2 to middle
	Eigen::Vector2d m_line2;

	Orientation orientation() const;

public:
	explicit Biarc(const Eigen::Vector2d &point1, const Eigen::Vector2d &middle, const Eigen::Vector2d &point2,
		const Eigen::Vector2d &tangent1, const Eigen::Vector2d &tangent2);

	const Eigen::Vector2d &middle() const;
	Eigen::Vector2d tangentAtMiddle() const;

	Polyline toPolyline() const;
};

}

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
	QVector2D m_point1;
	QVector2D m_point2;
	/// Intersection point of the arcs
	QVector2D m_middle;
	/// Tangent at point1
	QVector2D m_tangent1;
	/// Tangent at point1
	QVector2D m_tangent2;
	/// Line from point1 to middle
	QVector2D m_line1;
	/// Line from point2 to middle
	QVector2D m_line2;

	Orientation orientation() const;

public:
	explicit Biarc(const QVector2D &point1, const QVector2D &middle, const QVector2D &point2,
		const QVector2D &tangent1, const QVector2D &tangent2);

	const QVector2D &middle() const;
	QVector2D tangentAtMiddle() const;

	Polyline toPolyline() const;
};

}

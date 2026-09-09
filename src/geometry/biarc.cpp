module;

#include <cmath>
#include <QVector2D>
#include <QDebug>
#include <QtMath>

export module geometry.biarc;

import common.aggregable;
import geometry.bulge;
import geometry.polyline;
import geometry.utils;

export namespace geometry
{

class Biarc : public common::Aggregable<Biarc>
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

	Orientation orientation() const
	{
		const float det = (m_middle.x() - m_point1.x()) * (m_middle.y() + m_point1.y())
			+ (m_point2.x() - m_middle.x()) * (m_point2.y() + m_middle.y())
			+ (m_point1.x() - m_point2.x()) * (m_point1.y() + m_point2.y());

		return (det < 0.0f) ? Orientation::CW : Orientation::CCW;
	}

public:
	explicit Biarc(const QVector2D& point1, const QVector2D& middle, const QVector2D& point2, const QVector2D& tangent1,
				   const QVector2D& tangent2)
		: m_point1(point1)
		, m_point2(point2)
		, m_middle(middle)
		, m_tangent1(tangent1)
		, m_tangent2(tangent2)
		, m_line1(middle - point1)
		, m_line2(middle - point2)
	{
	}

	const QVector2D& middle() const { return m_middle; }

	QVector2D tangentAtMiddle() const
	{
		// Rotate line by PI/2
		const QVector2D normalizedLine1 = m_line1.normalized();
		const QVector2D perpendicularLine1 = PerpendicularLine(normalizedLine1);

		// Tangent at middle is the reflect of tangent at start by perpendicular line start to end.
		return ReflectLine(m_tangent1.normalized(), perpendicularLine1);
	}

	float approximateLength() const { return (m_point1 - m_middle).length() + (m_point2 - m_middle).length(); }

	Polyline toLinePolyline() const
	{
		const Bulge bulge(m_point1, m_point2, 0.0f);
		return Polyline({ bulge });
	}

	Polyline toPolyline() const
	{
		/* Angle from end to start line with arc tangent at start point is double of
		 * bulge tangent angle.
		 */

		// Half angle from tangent1 to line1
		const float thetab1 = (LineAngle(m_line1) - LineAngle(m_tangent1)) / 2.0f;
		// Half angle from line2 to tangent2
		const float thetab2 = (LineAngle(m_tangent2) - LineAngle(m_line2)) / 2.0f;

		const Bulge b1(m_point1, m_middle, std::tan(thetab1));
		const Bulge b2(m_middle, m_point2, std::tan(thetab2));

		return Polyline({ b1, b2 });
	}
};

}

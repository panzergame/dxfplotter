#pragma once

#include <common/aggregable.h>

#include <geometry/arc.h>
#include <geometry/circle.h>
#include <geometry/utils.h>

#include <QVector2D>

#include <cavc/plinesegment.hpp>

namespace Geometry
{

class Bulge : public Common::Aggregable<Bulge> // TODO rename segment and tangent bulge ?
{
private:
	QVector2D m_start;
	QVector2D m_end;

	float m_tangent;

public:
	/** Define a bulge
	 * @param start Starting point of the bulge
	 * @param end Ending point of the bulge
	 * @param tangent Tangent of a thourth of the arc angle.
	 * Negative tangent means the arc goes clockwise from start to end,
	 * otherwise anti clockwise from start to end.
	 */
	explicit Bulge(const QVector2D &start, const QVector2D &end, float tangent);
	explicit Bulge(const cavc::PlineVertex<double> &v1, const cavc::PlineVertex<double> &v2);
	explicit Bulge() = default;

	const QVector2D &start() const;
	const QVector2D &end() const;
	float tangent() const;

	float length() const;

	/// Change direction
	void invert();
	/// Transform to line, means tangent is 0.
	void linify();

	// Extend bulge start point
	Bulge extendStart(const QVector2D &start) const;
	// Extend bulge end point
	Bulge extendEnd(const QVector2D &end) const;

	bool isLine() const;
	bool isArc() const;
	Orientation orientation() const;

	Circle toCircle() const;
	Arc toArc() const;
};

}

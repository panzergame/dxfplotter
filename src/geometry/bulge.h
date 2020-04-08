#pragma once

#include <common/aggregable.h>

#include <geometry/arc.h>
#include <geometry/circle.h>
#include <geometry/utils.h>

#include <QVector2D>

namespace Geometry
{

class Bulge : public Common::Aggregable<Bulge>
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
	Bulge() = default;

	const QVector2D &start() const;
	const QVector2D &end() const;

	float length() const;

	void invert();

	Bulge extendStart(const QVector2D &start) const;
	Bulge extendEnd(const QVector2D &end) const;

	bool isLine() const;
	Orientation orientation() const;

	Circle toCircle() const;
	Arc toArc() const;
};

}

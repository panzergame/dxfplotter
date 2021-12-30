#pragma once

#include <common/aggregable.h>

#include <geometry/arc.h>
#include <geometry/circle.h>
#include <geometry/utils.h>

#include <cavc/plinesegment.hpp>

#include <serializer/access.h>

#include <QVector2D>
#include <QTransform>

namespace geometry
{

class Bulge : public Common::Aggregable<Bulge>
{
	friend serializer::Access<Bulge>;

private:
	Eigen::Vector2d m_start;
	Eigen::Vector2d m_end;

	double m_tangent;

public:
	/** Define a bulge
	 * @param start Starting point of the bulge
	 * @param end Ending point of the bulge
	 * @param tangent Tangent of a thourth of the arc angle.
	 * Negative tangent means the arc goes clockwise from start to end,
	 * otherwise anti clockwise from start to end.
	 */
	explicit Bulge(const Eigen::Vector2d &start, const Eigen::Vector2d &end, double tangent);
	explicit Bulge(const cavc::PlineVertex<double> &v1, const cavc::PlineVertex<double> &v2);
	explicit Bulge() = default;

	const Eigen::Vector2d &start() const;
	Eigen::Vector2d &start();
	const Eigen::Vector2d &end() const;
	Eigen::Vector2d &end();
	double tangent() const;
	double &tangent();

	double length() const;

	/// Change direction
	void invert();
	/// Transform to line, means tangent is 0.
	void linify();

	// Extend bulge start point
	Bulge extendStart(const Eigen::Vector2d &start) const;
	// Extend bulge end point
	Bulge extendEnd(const Eigen::Vector2d &end) const;

	bool isLine() const;
	bool isArc() const;
	Orientation orientation() const;

	Circle toCircle() const;
	Arc toArc() const;

	void transform(const Eigen::Affine2d &matrix);

	bool operator==(const Bulge& other) const;
};

}

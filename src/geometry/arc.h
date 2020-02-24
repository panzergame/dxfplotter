#pragma once

#include <QVector2D>

#include <geometry/utils.h>

namespace Geometry
{

class Arc
{
private:
	QVector2D m_center;
	QVector2D m_start;
	QVector2D m_end;
	float m_radius;
	float m_startAngle;
	float m_endAngle;
	float m_spanAngle;
	Orientation m_orientation;

public:
	explicit Arc(const QVector2D &center, const QVector2D &start, const QVector2D &end,
				 float radius, float starAngle, float endAngle, Orientation orientation);

	const QVector2D &center() const;
	const QVector2D &start() const;
	const QVector2D &end() const;
	Orientation orientation() const;
	float radius() const;
	float startAngle() const;
	float endAngle() const;
	float spanAngle() const;
};

};

#pragma once

#include <QVector2D>

namespace Model
{

class Arc
{
private:
	QVector2D m_center;
	float m_radius;
	float m_startAngle;
	float m_endAngle;
	float m_spanAngle;

public:
	explicit Arc(const QVector2D &center, float radius, float starAngle, float endAngle);

	const QVector2D &center() const;
	float radius() const;
	float startAngle() const;
	float endAngle() const;
	float spanAngle() const;
};

};

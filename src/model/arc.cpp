#include <arc.h>
#include <math.h>

#include <QDebug> // TODO

namespace Model
{

Arc::Arc(const QVector2D &center, float radius, float starAngle, float endAngle)
	:m_center(center),
	m_radius(radius),
	m_startAngle(starAngle),
	m_endAngle(endAngle),
	m_spanAngle(m_endAngle - m_startAngle)
{
	if (m_spanAngle < 0.0f) {
		m_spanAngle += M_PI * 2.0f;
	}
	qInfo() << m_spanAngle;
}

const QVector2D &Arc::center() const
{
	return m_center;
}

float Arc::radius() const
{
	return m_radius;
}

float Arc::startAngle() const
{
	return m_startAngle;
}

float Arc::endAngle() const
{
	return m_endAngle;
}

float Arc::spanAngle() const
{
	return m_spanAngle;
}

}

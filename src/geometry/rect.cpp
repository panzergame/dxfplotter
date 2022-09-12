#include <rect.h>

namespace geometry
{

QVector2D minVect(const QVector2D &v1, const QVector2D &v2)
{
	return {std::min(v1.x(), v2.x()), std::min(v1.y(), v2.y())};
}

QVector2D maxVect(const QVector2D &v1, const QVector2D &v2)
{
	return {std::max(v1.x(), v2.x()), std::max(v1.y(), v2.y())};	
}

Rect::Rect(const QVector2D &v)
	:m_min(v),
	m_max(v)
{
}

Rect::Rect(const QVector2D &v1, const QVector2D& v2)
	:m_min(minVect(v1, v2)),
	m_max(maxVect(v1, v2))
{
}

const QVector2D &Rect::min() const
{
	return m_min;
}

const QVector2D &Rect::max() const
{
	return m_max;
}

QVector2D Rect::bottomLeft() const
{
	return m_min;
}

QVector2D Rect::topLeft() const
{
	return QVector2D(m_min.x(), m_max.y());
}
QVector2D Rect::topRight() const
{
	return m_max;
}
QVector2D Rect::bottomRight() const
{
	return QVector2D(m_max.x(), m_min.y());
}

QRectF Rect::toQt() const
{
	const QPointF topLeft(m_min.x(), m_max.y());
	const QPointF bottomRight(m_max.x(), m_min.y());

	return QRectF(topLeft, bottomRight);
}

Rect Rect::operator|(const Rect& other) const
{
	return Rect(minVect(min(), other.min()), maxVect(max(), other.max()));
}

Rect &Rect::operator|=(const Rect& other)
{
	m_min = minVect(min(), other.min());
	m_max = maxVect(max(), other.max());
	return *this;
}

Rect Rect::operator+(const QVector2D &v)
{
	Rect extendedRect;
	extendedRect.m_min = minVect(m_min, v);
	extendedRect.m_max = maxVect(m_max, v);

	return extendedRect;
}

}

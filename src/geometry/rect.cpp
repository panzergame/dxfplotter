module;

#include <algorithm>
#include <QVector2D>
#include <QRectF>

export module geometry.rect;

namespace geometry
{

QVector2D minVect(const QVector2D& v1, const QVector2D& v2)
{
	return { std::min(v1.x(), v2.x()), std::min(v1.y(), v2.y()) };
}

QVector2D maxVect(const QVector2D& v1, const QVector2D& v2)
{
	return { std::max(v1.x(), v2.x()), std::max(v1.y(), v2.y()) };
}

}

export namespace geometry
{

class Rect
{
private:
	QVector2D m_min;
	QVector2D m_max;

public:
	Rect() = default;
	explicit Rect(const QVector2D& v)
		: m_min(v)
		, m_max(v)
	{
	}

	explicit Rect(const QVector2D& v1, const QVector2D& v2)
		: m_min(minVect(v1, v2))
		, m_max(maxVect(v1, v2))
	{
	}

	const QVector2D& min() const { return m_min; }

	const QVector2D& max() const { return m_max; }

	QVector2D bottomLeft() const { return m_min; }

	QVector2D topLeft() const { return QVector2D(m_min.x(), m_max.y()); }

	QVector2D topRight() const { return m_max; }

	QVector2D bottomRight() const { return QVector2D(m_max.x(), m_min.y()); }

	QRectF toQt() const
	{
		const QPointF topLeft(m_min.x(), m_max.y());
		const QPointF bottomRight(m_max.x(), m_min.y());

		return QRectF(topLeft, bottomRight);
	}

	Rect operator|(const Rect& other) const { return Rect(minVect(min(), other.min()), maxVect(max(), other.max())); }

	Rect& operator|=(const Rect& other)
	{
		m_min = minVect(min(), other.min());
		m_max = maxVect(max(), other.max());
		return *this;
	}

	Rect operator+(const QVector2D& v)
	{
		Rect extendedRect;
		extendedRect.m_min = minVect(m_min, v);
		extendedRect.m_max = maxVect(m_max, v);

		return extendedRect;
	}
};

}

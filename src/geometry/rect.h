#pragma once

#include "qvectornd.h"
#include <QVector2D>

namespace geometry
{

class Rect
{
private:
	QVector2D m_min;
	QVector2D m_max;

public:
	Rect() = default;
	explicit Rect(const QVector2D &v);
	explicit Rect(const QVector2D &v1, const QVector2D& v2);

	const QVector2D &min() const;
	const QVector2D &max() const;

	QVector2D bottomLeft() const;
	QVector2D topLeft() const;
	QVector2D topRight() const;
	QVector2D bottomRight() const;

	QRectF toQt() const;

	Rect operator|(const Rect& other) const;
	Rect &operator|=(const Rect& other);
	Rect operator+(const QVector2D &v);
};

}

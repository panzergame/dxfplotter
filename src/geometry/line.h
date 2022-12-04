#pragma once

#include <QVector2D>

namespace geometry
{

class Line
{
private:
	QVector2D m_start;
	QVector2D m_end;

public:
	explicit Line(const QVector2D &start, const QVector2D& end);

	const QVector2D &start() const;
	const QVector2D &end() const;

	bool lengthNonZero() const;
};

}

module;

#include <QVector2D>

export module geometry.line;

export namespace geometry
{

class Line
{
private:
	QVector2D m_start;
	QVector2D m_end;

public:
	explicit Line(const QVector2D& start, const QVector2D& end)
		: m_start(start)
		, m_end(end)
	{
	}

	const QVector2D& start() const { return m_start; }

	const QVector2D& end() const { return m_end; }

	bool lengthNonZero() const { return (m_start != m_end); }

	float length() const { return m_start.distanceToPoint(m_end); }
};

}

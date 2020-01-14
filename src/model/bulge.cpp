#include <bulge.h>

namespace Model
{

Bulge::Bulge(const QVector2D &start, const QVector2D &end, float tangent)
	:m_start(start),
	m_end(end),
	m_tangent(tangent)
{
}

const QVector2D &Bulge::start() const
{
	return m_start;
}

const QVector2D &Bulge::end() const
{
	return m_end;
}

void Bulge::invert()
{
	std::swap(m_start, m_end);
	m_tangent = -m_tangent;
}


}

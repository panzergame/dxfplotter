#include <bulge.h>
#include <cmath>
#include <limits>

#include <QDebug> // TODO

namespace Geometry
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

bool Bulge::isLine() const
{
	return std::abs(m_tangent) < std::numeric_limits<float>::epsilon();
}

Arc Bulge::toArc() const
{
	const QVector2D line = m_end - m_start;
	const float midline = line.length() / 2.0f;
	const float sagitta = midline * m_tangent;
	// WARNING It is not real radius because it can be negative
	const float radius = (sagitta * sagitta + midline * midline) / (2.0f * sagitta);
	const float lineAngle = std::atan2(line.y(), line.x());
	const float theta4 = std::atan(m_tangent);
	const float theta = theta4 * 4.0f;
	const float angleToCenter = lineAngle + (M_PI_2 - 2.0f * theta4);

	const QVector2D relativeCenter(std::cos(angleToCenter) * radius, std::sin(angleToCenter) * radius);
	const QVector2D center = relativeCenter + m_start;

// 	const float startAngle = std::atan2(-relativeCenter.y(), -relativeCenter.x());
// 	const float endAngle = m_tangent > 0.0f ? startAngle - theta : startAngle + theta; // TODO optimization


	const float startAngle = std::atan2(m_start.y() - center.y(), m_start.x() - center.x());
	const float endAngle = std::atan2(m_end.y() - center.y(), m_end.x() - center.x());

	return Arc(center, std::abs(radius), startAngle, endAngle);
}

}

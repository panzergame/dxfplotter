#include <polyline.h>
#include <utils.h>

namespace Core
{

Polyline::Polyline(Bulges &&bulges)
	:m_bulges(bulges)
{
}

const QVector2D &Polyline::start() const
{
	return m_bulges.front().start();
}

const QVector2D &Polyline::end() const
{
	return m_bulges.back().end();
}

Polyline &Polyline::invert()
{
	for (Bulge &bulge : m_bulges) {
		bulge.invert();
	}

	std::reverse(m_bulges.begin(), m_bulges.end());

	return *this;
}

Polyline& Polyline::operator+=(const Polyline &other)
{
	m_bulges.insert(m_bulges.end(), other.m_bulges.begin(), other.m_bulges.end());

	return *this;
}

std::ostream &operator<<(std::ostream &stream, const Polyline &polyline)
{
	stream << polyline.start();
	for (const Bulge &bulge : polyline.m_bulges) {
		stream << " -> " << bulge.end();
	}

	return stream;
}

}

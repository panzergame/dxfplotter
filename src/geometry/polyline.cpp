#include <polyline.h>
#include <utils.h>

namespace Geometry
{

Polyline::Polyline(const cavc::Polyline<double> &polyline)
{
	m_bulges.resize(polyline.isClosed() ? polyline.size() : polyline.size() - 1);

	polyline.visitSegIndices([&polyline, this](size_t i, size_t j){
		m_bulges[i] = Bulge(polyline[i], polyline[j]);
		return true;
	});
}

Polyline::Polyline(Bulge::List &&bulges)
	:m_bulges(bulges)
{
	assert(!m_bulges.empty());
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

Polyline::List Polyline::offsetted(float offset) const
{
	cavc::Polyline<double> ccPolyline;

	// Convert to CAVC polyline
	const Bulge &first = m_bulges.front();

	forEachBulge([&ccPolyline](const Bulge &bulge){
		const QVector2D &start = bulge.start();
		ccPolyline.addVertex(start.x(), start.y(), bulge.tangent());
	});

	// TODO open polyline

	ccPolyline.isClosed() = true;

	std::vector<cavc::Polyline<double> > offsetedCcPolylines = cavc::parallelOffset(ccPolyline, (double)offset);

	Polyline::List offsetedPolylines(offsetedCcPolylines.size());
	std::transform(offsetedCcPolylines.begin(), offsetedCcPolylines.end(), offsetedPolylines.begin(),
		[](const cavc::Polyline<double> &polyline) {
			return Polyline(polyline);
		});

	return offsetedPolylines;
}

}

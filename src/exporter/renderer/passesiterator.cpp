#include <passesiterator.h>

bool PassesIterator::needPolylineInverse() const
{
	return (!m_closed || m_cuttingBackward);
}

const geometry::Polyline &PassesIterator::polylineForward() const
{
	return m_cuttingBackward ? m_polylineInverse : m_polyline;
}

const geometry::Polyline &PassesIterator::polylineBackward() const
{
	return m_cuttingBackward ? m_polyline : m_polylineInverse;
}

PassesIterator::PassesIterator(const geometry::Polyline &polyline, geometry::CuttingDirection direction)
	:m_closed(polyline.isClosed()),
	m_cuttingBackward(direction == geometry::CuttingDirection::BACKWARD),
	m_polyline(polyline),
	m_polylineInverse(needPolylineInverse() ? m_polyline.inverse() : geometry::Polyline())
{
}

const geometry::Polyline &PassesIterator::operator*() const
{
	if (m_closed || m_odd) {
		return polylineForward();
	}
	return polylineBackward();
}

PassesIterator &PassesIterator::operator++()
{
	m_odd = !m_odd;

	return *this;
}


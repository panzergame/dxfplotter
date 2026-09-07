module;

#include <QVector2D>

export module exporter.renderer.passesiterator;

import geometry.polyline;
import geometry.utils;

// Return next polyline to convert
export class PassesIterator
{
private:
	bool m_odd = true;
	const bool m_closed;
	const bool m_cuttingBackward;
	const geometry::Polyline& m_polyline;
	const geometry::Polyline m_polylineInverse;

	bool needPolylineInverse() const;
	const geometry::Polyline& polylineForward() const;
	const geometry::Polyline& polylineBackward() const;

public:
	explicit PassesIterator(const geometry::Polyline& polyline, geometry::CuttingDirection direction);

	const geometry::Polyline& operator*() const;
	PassesIterator& operator++();
};

bool PassesIterator::needPolylineInverse() const
{
	return (!m_closed || m_cuttingBackward);
}

const geometry::Polyline& PassesIterator::polylineForward() const
{
	return m_cuttingBackward ? m_polylineInverse : m_polyline;
}

const geometry::Polyline& PassesIterator::polylineBackward() const
{
	return m_cuttingBackward ? m_polyline : m_polylineInverse;
}

PassesIterator::PassesIterator(const geometry::Polyline& polyline, geometry::CuttingDirection direction)
	: m_closed(polyline.isClosed())
	, m_cuttingBackward(direction == geometry::CuttingDirection::BACKWARD)
	, m_polyline(polyline)
	, m_polylineInverse(needPolylineInverse() ? m_polyline.inverse() : geometry::Polyline())
{
}

const geometry::Polyline& PassesIterator::operator*() const
{
	if (m_closed || m_odd) {
		return polylineForward();
	}
	return polylineBackward();
}

PassesIterator& PassesIterator::operator++()
{
	m_odd = !m_odd;

	return *this;
}

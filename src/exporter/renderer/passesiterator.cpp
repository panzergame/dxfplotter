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
	const bool m_alternate;
	const bool m_cuttingBackward;
	const geometry::Polyline& m_polyline;
	const geometry::Polyline m_polylineInverse;

	const geometry::Polyline& polylineForward() const { return m_cuttingBackward ? m_polylineInverse : m_polyline; }

	const geometry::Polyline& polylineBackward() const { return m_cuttingBackward ? m_polyline : m_polylineInverse; }

public:
	/** alternate: every other pass travels backward so an open polyline is cut back and forth from where the previous
	 * pass ended. A closed polyline ends where it starts and never alternates.
	 */
	explicit PassesIterator(const geometry::Polyline& polyline, geometry::CuttingDirection direction, bool alternate)
		: m_alternate(alternate && !polyline.isClosed())
		, m_cuttingBackward(direction == geometry::CuttingDirection::BACKWARD)
		, m_polyline(polyline)
		, m_polylineInverse((m_alternate || m_cuttingBackward) ? polyline.inverse() : geometry::Polyline())
	{
	}

	const geometry::Polyline& operator*() const
	{
		if (!m_alternate || m_odd) {
			return polylineForward();
		}
		return polylineBackward();
	}

	PassesIterator& operator++()
	{
		m_odd = !m_odd;

		return *this;
	}
};

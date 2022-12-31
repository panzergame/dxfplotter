#pragma once

#include <geometry/polyline.h>

// Return next polyline to convert
class PassesIterator
{
private:
	bool m_odd = true;
	const bool m_closed;
	const bool m_cuttingBackward;
	const geometry::Polyline& m_polyline;
	const geometry::Polyline m_polylineInverse;

	bool needPolylineInverse() const;
	const geometry::Polyline &polylineForward() const;
	const geometry::Polyline &polylineBackward() const;

public:
	explicit PassesIterator(const geometry::Polyline &polyline, geometry::CuttingDirection direction);

	const geometry::Polyline &operator*() const;
	PassesIterator &operator++();
};


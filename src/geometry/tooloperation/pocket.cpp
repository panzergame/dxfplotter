module;

#include <cstdint>
#include <utility>

export module geometry.tooloperation.pocket;

import geometry.filter.cleaner;
import geometry.pocketer;
import geometry.polyline;
import geometry.tooloperation.tooloperation;
import geometry.utils;

export namespace geometry::tooloperation
{

class Pocket : public ToolOperation
{
private:
	Polyline::ListCPtr m_islands;
	float m_radius;
	float m_minimumPolylineLength;
	float m_minimumArcLength;

public:
	explicit Pocket(const Polyline::ListCPtr& islands, float radius, float minimumPolylineLength,
					float minimumArcLength)
		: m_islands(islands)
		, m_radius(radius)
		, m_minimumPolylineLength(minimumPolylineLength)
		, m_minimumArcLength(minimumArcLength)
	{
	}

	Result generate(const Polyline& polyline) const override
	{
		Pocketer pocketer(polyline, m_islands, m_radius, m_minimumPolylineLength);
		filter::Cleaner cleaner(std::move(pocketer.polylines()), m_minimumPolylineLength, m_minimumArcLength);

		// Pocket is left when polyline is CCW winded.
		static const CuttingDirection borderOrientationToPocketDirection[] = {
			CuttingDirection::BACKWARD, // Orientation::CW
			CuttingDirection::FORWARD // Orientation::CCW
		};
		const CuttingDirection direction
			= borderOrientationToPocketDirection[static_cast<int>(pocketer.borderOrientation())];

		return { ToolPolyline::FromPolylines(cleaner.polylines()), direction };
	}
};

}

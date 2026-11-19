module;

#include <cstdint>
#include <utility>

export module geometry.tooloperation.offset;

import geometry.filter.cleaner;
import geometry.polyline;
import geometry.tooloperation.tooloperation;
import geometry.utils;

export namespace geometry::tooloperation
{

class Offset : public ToolOperation
{
private:
	float m_margin;
	float m_minimumPolylineLength;
	float m_minimumArcLength;

public:
	explicit Offset(float margin, float minimumPolylineLength, float minimumArcLength)
		: m_margin(margin)
		, m_minimumPolylineLength(minimumPolylineLength)
		, m_minimumArcLength(minimumArcLength)
	{
	}

	Result generate(const Polyline& polyline) const override
	{
		Polyline::List offsettedPolylines = polyline.offsetted(m_margin);
		filter::Cleaner cleaner(std::move(offsettedPolylines), m_minimumPolylineLength, m_minimumArcLength);

		const CuttingDirection direction = (m_margin > 0.0f) ? CuttingDirection::FORWARD : CuttingDirection::BACKWARD;

		return { cleaner.polylines(), direction };
	}
};

}

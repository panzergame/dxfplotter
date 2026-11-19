module;

export module geometry.tooloperation.dragknife;

import geometry.polyline;
import geometry.tooloperation.tooloperation;
import geometry.utils;

export namespace geometry::tooloperation
{

/** Holder path of a drag knife whose blade tip trails by bladeOffset.
 * Compensation depends on travel direction, so the polyline is inverted beforehand for a backward profile cut
 * direction; returning that same direction cancels the inversion done at export.
 */
class DragKnife : public ToolOperation
{
private:
	float m_bladeOffset;
	float m_minimumSwivelAngle;
	CuttingDirection m_profileDirection;

public:
	explicit DragKnife(float bladeOffset, float minimumSwivelAngle, CuttingDirection profileDirection)
		: m_bladeOffset(bladeOffset)
		, m_minimumSwivelAngle(minimumSwivelAngle)
		, m_profileDirection(profileDirection)
	{
	}

	Result generate(const Polyline& polyline) const override
	{
		Polyline directed = polyline;
		if (m_profileDirection == CuttingDirection::BACKWARD) {
			directed.invert();
		}

		return { { directed.dragKnifeCompensated(m_bladeOffset, m_minimumSwivelAngle) }, m_profileDirection };
	}
};

}

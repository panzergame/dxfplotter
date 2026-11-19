module;

export module geometry.tooloperation.dragknife;

import geometry.polyline;
import geometry.tooloperation.tooloperation;
import geometry.utils;

export namespace geometry::tooloperation
{

/** Holder path of a drag knife whose blade tip trails by bladeOffset.
 * sideDirection plays the role of the side for Offset: the path is travelled along
 * sideDirection | profileDirection. Compensation depends on travel direction, so the polyline is inverted
 * beforehand when travelling backward; returning the profile direction cancels the inversion done at export.
 */
class DragKnife : public ToolOperation
{
private:
	float m_bladeOffset;
	float m_minimumSwivelAngle;
	CuttingDirection m_sideDirection;
	CuttingDirection m_profileDirection;

public:
	explicit DragKnife(float bladeOffset, float minimumSwivelAngle, CuttingDirection sideDirection,
		CuttingDirection profileDirection)
		: m_bladeOffset(bladeOffset)
		, m_minimumSwivelAngle(minimumSwivelAngle)
		, m_sideDirection(sideDirection)
		, m_profileDirection(profileDirection)
	{
	}

	Result generate(const Polyline& polyline) const override
	{
		Polyline directed = polyline;
		if ((m_sideDirection | m_profileDirection) == CuttingDirection::BACKWARD) {
			directed.invert();
		}

		return { { directed.dragKnifeCompensated(m_bladeOffset, m_minimumSwivelAngle) }, m_profileDirection };
	}
};

}

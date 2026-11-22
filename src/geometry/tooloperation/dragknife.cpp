module;

#include <utility>

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
 * The blade heading is unknown before cutting, so the entry is an alignment stroke of alignmentLength. A knife has
 * no width, the side instead designates the waste side, as the material an offset tool would remove, where the
 * stroke of a closed polyline is kept.
 */
class DragKnife : public ToolOperation
{
private:
	float m_bladeOffset;
	float m_minimumSwivelAngle;
	float m_alignmentLength;
	CuttingDirection m_sideDirection;
	CuttingDirection m_profileDirection;

public:
	explicit DragKnife(float bladeOffset, float minimumSwivelAngle, float alignmentLength,
					   CuttingDirection sideDirection, CuttingDirection profileDirection)
		: m_bladeOffset(bladeOffset)
		, m_minimumSwivelAngle(minimumSwivelAngle)
		, m_alignmentLength(alignmentLength)
		, m_sideDirection(sideDirection)
		, m_profileDirection(profileDirection)
	{
	}

	Result generate(const Polyline& polyline) const override
	{
		const bool inverted = (m_sideDirection | m_profileDirection) == CuttingDirection::BACKWARD;
		const Polyline directed = inverted ? polyline : polyline.inverse();

		// Forward side is the left one, as for Offset, and inversion swaps sides relative to travel.
		const bool wasteOnLeft = (m_sideDirection == CuttingDirection::FORWARD) != inverted;

		ToolPolyline toolPolyline { directed.dragKnifeCompensated(m_bladeOffset, m_minimumSwivelAngle),
									directed.dragKnifeAlignment(m_bladeOffset, m_alignmentLength, wasteOnLeft) };

		return { { std::move(toolPolyline) }, m_profileDirection };
	}
};

}

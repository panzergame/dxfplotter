module;

#include <optional>
#include <utility>

export module geometry.tooloperation.dragknife;

import geometry.polyline;
import geometry.tooloperation.dragknifeentry;
import geometry.tooloperation.tooloperation;
import geometry.utils;

export namespace geometry::tooloperation
{

/** Holder path of a drag knife whose blade tip trails by bladeOffset.
 * sideDirection plays the role of the side for Offset: the path is travelled along
 * sideDirection | profileDirection. Compensation depends on travel direction, so the polyline is inverted
 * beforehand when travelling backward; returning the profile direction cancels the inversion done at export.
 * The blade heading is unknown before cutting, so the entry is an L shaped pair of strokes aligning every heading
 * outside π ± flipTolerance within alignmentTolerance of the start tangent. A knife has no width, the side instead
 * designates the waste side, as the material an offset tool would remove, where the entry may mark.
 */
class DragKnife : public ToolOperation
{
private:
	float m_minimumSwivelAngle;
	DragKnifeLengths m_entryLengths;
	CuttingDirection m_sideDirection;
	CuttingDirection m_profileDirection;

public:
	explicit DragKnife(float bladeOffset, float minimumSwivelAngle, float flipTolerance, float alignmentTolerance,
					   CuttingDirection sideDirection, CuttingDirection profileDirection)
		: m_minimumSwivelAngle(minimumSwivelAngle)
		, m_entryLengths(DragKnifeEntryLengths(bladeOffset, flipTolerance, alignmentTolerance))
		, m_sideDirection(sideDirection)
		, m_profileDirection(profileDirection)
	{
	}

	Result generate(const Polyline& polyline) const override
	{
		const bool inverted = (m_sideDirection | m_profileDirection) == CuttingDirection::BACKWARD;
		const Polyline directed = inverted ? polyline.inverse() : polyline;

		// Forward side is the left one, as for Offset, and inversion swaps sides relative to travel.
		const bool wasteOnLeft = (m_sideDirection == CuttingDirection::FORWARD) != inverted;

		ToolPolyline toolPolyline { directed.dragKnifeCompensated(m_entryLengths.bladeOffset, m_minimumSwivelAngle),
									DragKnifeEntrySelect(directed, wasteOnLeft, m_entryLengths)
										.transform([this](const DragKnifeEntryCandidate& candidate) {
											return DragKnifeEntry(candidate, m_entryLengths);
										}) };

		return { { std::move(toolPolyline) }, m_profileDirection };
	}
};

}

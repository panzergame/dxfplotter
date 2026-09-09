module;

#include <variant>
#include <vector>
#include <QVector3D>
import exporter.renderer.renderer;

#include <QDebug>

export module model.simulation;

import common.aggregable;
import config.config;
import geometry.arc;
import geometry.bulge;
import geometry.line;
import geometry.polyline;
import geometry.utils;
import model.document;
namespace model
{

float mmPerMinTommPerSec(float mmPerMin)
{
	return mmPerMin / 60.0f;
}

float entityDuration(const geometry::Line& line, float feedRateMmPerMin)
{
	return line.length() / mmPerMinTommPerSec(feedRateMmPerMin);
}

float entityDuration(float start, float end, float feedRateMmPerMin)
{
	return std::abs(end - start) / mmPerMinTommPerSec(feedRateMmPerMin);
}

float entityDuration(const geometry::Arc& arc, float feedRateMmPerMin)
{
	return arc.length() / mmPerMinTommPerSec(feedRateMmPerMin);
}

}

export namespace model
{

class Simulation : public common::Aggregable<Simulation>
{
public:
	enum class MoveType { NormalWithCut = 0, FastWithoutCut };

	struct ToolPathPoint3D : common::Aggregable<ToolPathPoint3D>
	{
		QVector3D position;
		MoveType moveType;

		ToolPathPoint3D() = default;
		explicit ToolPathPoint3D(const QVector3D& position, model::Simulation::MoveType moveType)
			: position(position)
			, moveType(moveType)
		{
		}
	};

private:
	class Traversable
	{
	protected:
		float m_startTime;
		float m_duration;
		MoveType m_moveType;

		float timeFactor(float time) const { return (time - m_startTime) / m_duration; }

	public:
		explicit Traversable(float startTime, float duration, MoveType moveType)
			: m_startTime(startTime)
			, m_duration(duration)
			, m_moveType(moveType)
		{
		}

		float startTime() const { return m_startTime; }

		float endTime() const { return m_startTime + m_duration; }

		float duration() const { return m_duration; }
	};

	class PlaneLineMotion : public Traversable
	{
	private:
		geometry::Line m_line;
		float m_depth;

	public:
		explicit PlaneLineMotion(float depth, const geometry::Line& line, float feedRate, float startTime,
								 MoveType moveType)
			: Traversable(startTime, entityDuration(line, feedRate), moveType)
			, m_line(line)
			, m_depth(depth)
		{
		}

		Simulation::ToolPathPoint3D pointAtTime(float time) const
		{
			const QVector2D planePos = geometry::Lerp(m_line.start(), m_line.end(), timeFactor(time));
			return ToolPathPoint3D(QVector3D(planePos, m_depth), m_moveType);
		}

		float endDepth() const { return m_depth; }

		const QVector2D& endPlanePos() const { return m_line.end(); }

		template<class Visitor>
		void approximateToLinesVisit(float maxError, Visitor&& visitor) const
		{
			visitor(QVector3D(m_line.end(), m_depth), m_moveType);
		}
	};

	class PlaneArcMotion : public Traversable
	{
	private:
		geometry::Arc m_arc;
		float m_depth;

	public:
		explicit PlaneArcMotion(float depth, const geometry::Arc& arc, float feedRate, float startTime,
								MoveType moveType)
			: Traversable(startTime, entityDuration(arc, feedRate), moveType)
			, m_arc(arc)
			, m_depth(depth)
		{
		}

		Simulation::ToolPathPoint3D pointAtTime(float time) const
		{
			const float angle = geometry::Lerp(m_arc.startAngle(), m_arc.endAngle(), timeFactor(time));
			const QVector2D planePos = m_arc.pointAtAngle(angle);
			return ToolPathPoint3D(QVector3D(planePos, m_depth), m_moveType);
		}

		float endDepth() const { return m_depth; }

		const QVector2D& endPlanePos() const { return m_arc.end(); }

		template<class Visitor>
		void approximateToLinesVisit(float maxError, Visitor&& visitor) const
		{
			m_arc.approximateToLinesVisit(maxError, [this, &visitor](const QVector2D& point) {
				visitor(QVector3D(point, m_depth), m_moveType);
			});
		}
	};

	class DepthMotion : public Traversable
	{
	private:
		QVector2D m_planePos;
		float m_fromDepth;
		float m_toDepth;

	public:
		explicit DepthMotion(const QVector2D& planePos, float fromDepth, float toDepth, float feedRate, float startTime,
							 MoveType moveType)
			: Traversable(startTime, entityDuration(fromDepth, toDepth, feedRate), moveType)
			, m_planePos(planePos)
			, m_fromDepth(fromDepth)
			, m_toDepth(toDepth)
		{
		}

		Simulation::ToolPathPoint3D pointAtTime(float time) const
		{
			const float depth = geometry::Lerp(m_fromDepth, m_toDepth, timeFactor(time));
			return ToolPathPoint3D(QVector3D(m_planePos, depth), m_moveType);
		}

		float endDepth() const { return m_toDepth; }

		const QVector2D& endPlanePos() const { return m_planePos; }

		template<class Visitor>
		void approximateToLinesVisit(float maxError, Visitor&& visitor) const
		{
			visitor(QVector3D(m_planePos, m_toDepth), m_moveType);
		}
	};

	using Motion = std::variant<PlaneLineMotion, PlaneArcMotion, DepthMotion>;

	using MotionList = std::vector<Motion>;

	class RenderVisitor
	{
	private:
		const float m_fastMoveFeedRate;

		MotionList m_motions;

		QVector2D m_cursorInPlane;
		float m_cursorDepth;
		float m_cursorTime;

		void initCursor(const QVector2D& to, float depth)
		{
			m_cursorInPlane = to;
			m_cursorDepth = depth;
			m_cursorTime = 0.0f;
		}

		void addMotion(Motion&& motion)
		{
			m_cursorTime = std::visit(
				[](auto& arg) {
					return arg.endTime();
				},
				motion);
			m_cursorDepth = std::visit(
				[](auto& arg) {
					return arg.endDepth();
				},
				motion);
			m_cursorInPlane = std::visit(
				[](auto& arg) {
					return arg.endPlanePos();
				},
				motion);
			m_motions.push_back(std::move(motion));
		}

		void linearMoveCursorInDepthTo(float toDepth, float feedRate, MoveType moveType)
		{
			addMotion(DepthMotion(m_cursorInPlane, m_cursorDepth, toDepth, feedRate, m_cursorTime, moveType));
		}

		void linearMoveCursorInPlaneTo(const QVector2D& to, float feedRate, MoveType moveType)
		{
			const geometry::Line line(m_cursorInPlane, to);
			if (line.lengthNonZero()) {
				addMotion(PlaneLineMotion(m_cursorDepth, line, feedRate, m_cursorTime, moveType));
			}
		}

		void moveCursorInPlaneAlong(const geometry::Line& line, float feedRate, MoveType moveType)
		{
			if (line.lengthNonZero()) {
				addMotion(PlaneLineMotion(m_cursorDepth, line, feedRate, m_cursorTime, moveType));
			}
		}

		void moveCursorInPlaneAlong(const geometry::Arc& arc, float feedRate, MoveType moveType)
		{
			addMotion(PlaneArcMotion(m_cursorDepth, arc, feedRate, m_cursorTime, moveType));
		}

	public:
		explicit RenderVisitor(float fastMoveFeedRate)
			: m_fastMoveFeedRate(fastMoveFeedRate)
		{
		}

		void start(const QVector2D& from, float safetyDepth)
		{
			initCursor(from, 0.0f);
			linearMoveCursorInDepthTo(safetyDepth, m_fastMoveFeedRate, MoveType::FastWithoutCut);
		}

		void end(const QVector2D& to, float safetyDepth)
		{
			linearMoveCursorInPlaneTo(to, m_fastMoveFeedRate, MoveType::FastWithoutCut);
		}

		void startOperation(const QVector2D& to, float intensity)
		{
			linearMoveCursorInPlaneTo(to, m_fastMoveFeedRate, MoveType::FastWithoutCut);
		}

		void endOperation(float safetyDepth)
		{
			linearMoveCursorInDepthTo(safetyDepth, m_fastMoveFeedRate, MoveType::FastWithoutCut);
		}

		void processPathAtDepth(const geometry::Polyline& polyline, float depth, float planeFeedRate,
								float depthFeedRate)
		{
			linearMoveCursorInDepthTo(depth, depthFeedRate, MoveType::NormalWithCut);

			polyline.forEachBulge([this, planeFeedRate](const geometry::Bulge& bulge) {
				if (bulge.isArc()) {
					moveCursorInPlaneAlong(bulge.toArc(), planeFeedRate, MoveType::NormalWithCut);
				} else {
					moveCursorInPlaneAlong(bulge.toLine(), planeFeedRate, MoveType::NormalWithCut);
				}
			});
		}

		MotionList&& motions() { return std::move(m_motions); }
	};
	MotionList m_motions;
	float m_duration;
	float m_toolRadius;

	const Simulation::Motion& findMotionAtTime(float time) const
	{
		MotionList::const_iterator motionIt
			= std::lower_bound(m_motions.begin(), m_motions.end(), time, [](const Motion& motionVariant, float time) {
				  const float endTime = std::visit(
					  [](auto& motion) {
						  return motion.endTime();
					  },
					  motionVariant);
				  return endTime < time;
			  });

		return *motionIt;
	}

	static Simulation::MotionList renderDocumentToMotions(const Document& document, float fastMoveFeedRate)
	{
		const config::Tools::Tool& tool = document.toolConfig();
		const config::Profiles::Profile& profile = document.profileConfig();

		RenderVisitor visitor(fastMoveFeedRate);
		exporter::renderer::Renderer renderer(tool, profile, visitor);

		renderer.render(document);

		return visitor.motions();
	}

	static float totalDurationOfMotions(const MotionList& motions)
	{
		const Motion& lastMotion = motions.back();
		return std::visit(
			[](const auto& motion) {
				return motion.endTime();
			},
			lastMotion);
	}

public:
	Simulation() = default;
	explicit Simulation(const Document& document, float fastMoveFeedRate)
		: m_motions(renderDocumentToMotions(document, fastMoveFeedRate))
		, m_duration(totalDurationOfMotions(m_motions))
		, m_toolRadius(document.toolConfig().general().radius())
	{
	}

	Simulation::ToolPathPoint3D toolPositionAtTime(float time) const
	{
		const Motion& motion = findMotionAtTime(time);
		return std::visit(
			[time](auto& arg) {
				return arg.pointAtTime(time);
			},
			motion);
	}

	float duration() const { return m_duration; }

	float toolRadius() const { return m_toolRadius; }

	Simulation::ToolPathPoint3D::List approximatedToolPathToLines(float maxError) const
	{
		ToolPathPoint3D::List points;
		auto pushBackPoint = [&points](const QVector3D& point, MoveType moveType) {
			points.emplace_back(point, moveType);
		};

		const QVector3D home(0.0f, 0.0f, 0.0f);
		pushBackPoint(home, MoveType::FastWithoutCut);

		std::for_each(m_motions.begin(), m_motions.end(), [&pushBackPoint, maxError](const Motion& motionVariant) {
			std::visit(
				[&pushBackPoint, maxError](const auto& motion) {
					motion.approximateToLinesVisit(maxError, pushBackPoint);
				},
				motionVariant);
		});

		return points;
	}
};

}

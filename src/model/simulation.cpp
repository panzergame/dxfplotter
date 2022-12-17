#include <simulation.h>
#include <document.h>
#include <exporter/renderer/renderer.h>

#include <QDebug> // TODO

namespace model
{

float entityDuration(const geometry::Line &line, float feedRate)
{
	return line.length() / feedRate;
}

float entityDuration(float start, float end, float feedRate)
{
	return std::abs(end - start) / feedRate;
}

float entityDuration(const geometry::Arc &arc, float feedRate)
{
	return arc.length() / feedRate;
}

Simulation::Traversable::Traversable(float startTime, float duration)
	:m_startTime(startTime),
	m_duration(duration)
{
}

float Simulation::Traversable::startTime() const
{
	return m_startTime;
}

float Simulation::Traversable::endTime() const
{
	return m_startTime + m_duration;
}

float Simulation::Traversable::duration() const
{
	return m_duration;
}

Simulation::PlaneLineMotion::PlaneLineMotion(float depth, const geometry::Line &line, float feedRate, float startTime)
	:Traversable(startTime, entityDuration(line, feedRate)),
	m_line(line),
	m_depth(depth)
{
}

float Simulation::PlaneLineMotion::endDepth() const
{
	return m_depth;
}

const QVector2D& Simulation::PlaneLineMotion::endPlanePos() const
{
	return m_line.end();
}

Simulation::PlaneArcMotion::PlaneArcMotion(float depth, const geometry::Arc &arc, float feedRate, float startTime)
	:Traversable(startTime, entityDuration(arc, feedRate)),
	m_arc(arc),
	m_depth(depth)
{
}

float Simulation::PlaneArcMotion::endDepth() const
{
	return m_depth;
}

const QVector2D& Simulation::PlaneArcMotion::endPlanePos() const
{
	return m_arc.end();
}

Simulation::DepthMotion::DepthMotion(const QVector2D &planePos, float fromDepth, float toDepth, float feedRate, float startTime)
	:Traversable(startTime, entityDuration(fromDepth, toDepth, feedRate)),
	m_planePos(planePos),
	m_fromDepth(fromDepth),
	m_toDepth(toDepth)
{
}

float Simulation::DepthMotion::endDepth() const
{
	return m_toDepth;
}

const QVector2D& Simulation::DepthMotion::endPlanePos() const
{
	return m_planePos;
}

class Simulation::RenderVisitor
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
		m_cursorTime = std::visit([](auto &arg){ return arg.endTime(); }, motion);
		m_cursorDepth = std::visit([](auto &arg){ return arg.endDepth(); }, motion);
		m_cursorInPlane = std::visit([](auto &arg){ return arg.endPlanePos(); }, motion);
		m_motions.push_back(std::move(motion));
	}

	void linearMoveCursorInDepthTo(float toDepth, float feedRate)
	{
		addMotion(DepthMotion(m_cursorInPlane, m_cursorDepth, toDepth, feedRate, m_cursorTime));
	}

	void linearMoveCursorInPlaneTo(const QVector2D& to, float feedRate)
	{
		const geometry::Line line(m_cursorInPlane, to);
		if (line.lengthNonZero()) {
			addMotion(PlaneLineMotion(m_cursorDepth, line, feedRate, m_cursorTime));
		}
	}

	void moveCursorInPlaneAlong(const geometry::Line &line, float feedRate)
	{
		if (line.lengthNonZero()) {
			addMotion(PlaneLineMotion(m_cursorDepth, line, feedRate, m_cursorTime));
		}
	}

	void moveCursorInPlaneAlong(const geometry::Arc &arc, float feedRate)
	{
		addMotion(PlaneArcMotion(m_cursorDepth, arc, feedRate, m_cursorTime));
	}

public:
	explicit RenderVisitor(float fastMoveFeedRate)
		:m_fastMoveFeedRate(fastMoveFeedRate)
	{
	}

	void start(const QVector2D& from, float safetyDepth)
	{
		initCursor(from, 0.0f);
		linearMoveCursorInDepthTo(safetyDepth, m_fastMoveFeedRate);
	}

	void end(const QVector2D& to, float safetyDepth)
	{
		linearMoveCursorInPlaneTo(to, m_fastMoveFeedRate);
	}

	void startOperation(const QVector2D& to, float intensity)
	{
		linearMoveCursorInPlaneTo(to, m_fastMoveFeedRate);
	}

	void endOperation(float safetyDepth)
	{
		linearMoveCursorInDepthTo(safetyDepth, m_fastMoveFeedRate);
	}

	void processPathAtDepth(const geometry::Polyline& polyline, float depth, float planeFeedRate, float depthFeedRate)
	{
		linearMoveCursorInDepthTo(depth, depthFeedRate);

		polyline.forEachBulge([this, planeFeedRate](const geometry::Bulge& bulge){
			if (bulge.isArc()) {
				moveCursorInPlaneAlong(bulge.toArc(), planeFeedRate);
			}
			else {
				moveCursorInPlaneAlong(bulge.toLine(), planeFeedRate);
			}
		});
	}

	MotionList &&motions()
	{
		return std::move(m_motions);
	}
};


Simulation::MotionList Simulation::renderDocumentToMotions(const Document &document) const
{
	const config::Tools::Tool& tool = document.toolConfig();
	const config::Profiles::Profile& profile = document.profileConfig();

	RenderVisitor visitor(42); // TODO fastfeedRate config
	exporter::renderer::Renderer renderer(tool, profile, visitor);

	renderer.render(document);

	return visitor.motions();
}

Simulation::Simulation(const Document &document)
	:m_motions(renderDocumentToMotions(document))
{
}

geometry::Point3DList Simulation::approximatedPathToLines(float maxError) const
{
	geometry::Point3DList points;
	auto pushBackPoint = [&points](const QVector3D &point){ points.push_back(point); };

	std::for_each(m_motions.begin(), m_motions.end(), [&pushBackPoint, maxError](const Motion& motionVariant){
		std::visit([&pushBackPoint, maxError](const auto &motion){
			motion.approximateToLinesVisit(maxError, pushBackPoint);
		}, motionVariant);
	});

	return points;
}

}


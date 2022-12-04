#include <simulation.h>
#include <document.h>

namespace model
{

class Simulation::RenderVisitor
{
private:
	const float m_fastMoveFeedRate;

	MotionList m_motions;

	QVector2D m_cursorInPlane;
	float m_cursorDepth;

	void initCursor(const QVector2D& to, float depth)
	{
		m_cursorInPlane = to;
		m_cursorDepth = depth;
	}

	void linearMoveCursorInDepthTo(float toDepth, float feedRate)
	{
		m_motions.push_back(DepthMotion(m_cursorDepth, toDepth, feedRate));
		m_cursorDepth = toDepth;
	}

	void linearMoveCursorInPlaneTo(const QVector2D& to, float feedRate)
	{
		const geometry::Line line(m_cursorInPlane, to);
		if (line.lengthNonZero()) {
			m_motions.push_back(PlaneLineMotion(m_cursorDepth, feedRate, line));
			m_cursorInPlane = to;
		}
	}

	void moveCursorInPlaneAlong(const geometry::Line &line, float feedRate)
	{
		if (line.lengthNonZero()) {
			m_motions.push_back(PlaneLineMotion(m_cursorDepth, feedRate, line));
			m_cursorInPlane = line.end();
		}
	}

	void moveCursorInPlaneAlong(const geometry::Arc &arc, float feedRate)
	{
		m_motions.push_back(PlaneArcMotion(m_cursorDepth, feedRate, arc));
		m_cursorInPlane = arc.end();
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
		linearMoveCursorInDepthTo(safetyDepth, m_fastMoveFeedRate);
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
};

Simulation::Simulation(Document &document)
{
	
}

}


#pragma once

#include <QVector3D>
#include <variant>

#include <geometry/arc.h>
#include <geometry/line.h>

#include <config/config.h>
#include <common/aggregable.h>

namespace model
{

class Document;

class Simulation : public common::Aggregable<Simulation>
{
public:
	enum class MoveType
	{
		NormalWithCut = 0,
		FastWithoutCut
	};

	struct ToolPathPoint3D : common::Aggregable<ToolPathPoint3D>
	{
		QVector3D position;
		MoveType moveType;

		ToolPathPoint3D() = default;
		explicit ToolPathPoint3D(const QVector3D& position, MoveType moveType);
	};

private:
	class RenderVisitor;

	class Traversable
	{
	protected:
		float m_startTime;
		float m_duration;
		MoveType m_moveType;

		float timeFactor(float time) const;

	public:
		explicit Traversable(float startTime, float duration, MoveType moveType);

		float startTime() const;
		float endTime() const;
		float duration() const;
	};

	class PlaneLineMotion : public Traversable
	{
	private:
		geometry::Line m_line;
		float m_depth;

	public:
		explicit PlaneLineMotion(float depth, const geometry::Line &line, float feedRate, float startTime, MoveType moveType);

		ToolPathPoint3D pointAtTime(float time) const;

		float endDepth() const;
		const QVector2D& endPlanePos() const;

		template <class Visitor>
		void approximateToLinesVisit(float maxError, Visitor &&visitor) const
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
		explicit PlaneArcMotion(float depth, const geometry::Arc &arc, float feedRate, float startTime, MoveType moveType);

		ToolPathPoint3D pointAtTime(float time) const;

		float endDepth() const;
		const QVector2D& endPlanePos() const;

		template <class Visitor>
		void approximateToLinesVisit(float maxError, Visitor &&visitor) const
		{
			m_arc.approximateToLinesVisit(maxError, [this, &visitor](const QVector2D &point){
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
		explicit DepthMotion(const QVector2D &planePos, float fromDepth, float toDepth, float feedRate, float startTime, MoveType moveType);

		ToolPathPoint3D pointAtTime(float time) const;

		float endDepth() const;
		const QVector2D& endPlanePos() const;

		template <class Visitor>
		void approximateToLinesVisit(float maxError, Visitor &&visitor) const
		{
			visitor(QVector3D(m_planePos, m_toDepth), m_moveType);
		}
	};

	using Motion = std::variant<PlaneLineMotion, PlaneArcMotion, DepthMotion>;

	using MotionList = std::vector<Motion>;
	MotionList m_motions;
	float m_duration;
	float m_toolRadius;

	const Motion &findMotionAtTime(float time) const;

	static MotionList renderDocumentToMotions(const Document &document);
	static float totalDurationOfMotions(const MotionList& motions);

public:
	Simulation() = default;
	explicit Simulation(const Document &document);

	ToolPathPoint3D toolPositionAtTime(float time) const;
	float duration() const;

	float toolRadius() const;

	ToolPathPoint3D::List approximatedToolPathToLines(float maxError) const;
};

}

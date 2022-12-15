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
private:
	class RenderVisitor;

	class Traversable
	{
	protected:
		float m_startTime;
		float m_duration;

	public:
		explicit Traversable(float startTime, float duration);

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
		explicit PlaneLineMotion(float depth, const geometry::Line &line, float feedRate, float startTime);

		QVector3D pointAtTime(float time) const;

		float endDepth() const;
		const QVector2D& endPlanePos() const;
	};

	class PlaneArcMotion : public Traversable
	{
	private:
		geometry::Arc m_arc;
		float m_depth;

	public:
		explicit PlaneArcMotion(float depth, const geometry::Arc &arc, float feedRate, float startTime);

		QVector3D pointAtTime(float time) const;

		float endDepth() const;
		const QVector2D& endPlanePos() const;
	};

	class DepthMotion : public Traversable
	{
	private:
		QVector2D m_planePos;
		float m_fromDepth;
		float m_toDepth;

	public:
		explicit DepthMotion(const QVector2D &planePos, float fromDepth, float toDepth, float feedRate, float startTime);

		QVector3D pointAtTime(float time) const;

		float endDepth() const;
		const QVector2D& endPlanePos() const;
	};

	using Motion = std::variant<PlaneLineMotion, PlaneArcMotion, DepthMotion>;

	using MotionList = std::vector<Motion>;
	MotionList m_motions;

	const Motion &findMotionAtTime(float time) const;

	MotionList renderDocumentToMotions(const Document &document) const;

public:
	explicit Simulation(const Document &document);

	QVector3D position(float time);
	float duration() const;

	geometry::Point3DList approximatedPathToLines(float maxError) const;
};

}

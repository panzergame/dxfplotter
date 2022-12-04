#pragma once

#include <QVector3D>
#include <variant>

#include <geometry/arc.h>
#include <geometry/line.h>

namespace model
{

class Document;

class Simulation
{
private:
	class RenderVisitor;

	class Traversable
	{
	protected:

	public:
		explicit Traversable();
	};

	class PlaneLineMotion : Traversable
	{
	private:
		geometry::Line m_line;
		float m_depth;

	public:
		explicit PlaneLineMotion(float depth, float feedRate, const geometry::Line &line);

		QVector3D pointAtTime(float time) const;
	};

	class PlaneArcMotion : Traversable
	{
	private:
		geometry::Arc m_arc;
		float m_depth;

	public:
		explicit PlaneArcMotion(float depth, float feedRate, const geometry::Arc &arc);

		QVector3D pointAtTime(float time) const;
	};

	class DepthMotion : Traversable
	{
	private:
		

	public:
		explicit DepthMotion(float fromDepth, float toDepth, float feedRate);

		QVector3D pointAtTime(float time) const;
	};

	using Motion = std::variant<PlaneLineMotion, PlaneArcMotion, DepthMotion>;

	using MotionList = std::vector<Motion>;
	MotionList m_motions;

	const Motion &findMotionAtTime(float time) const;

public:
	explicit Simulation(Document &document);

	QVector3D position(float time);
	float duration() const;
};

}

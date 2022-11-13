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
		float m_startTime;
		float m_endTime;
		float m_depth;

	public:
		explicit Traversable(float startTime, float endTime, float depth);
	};

	class LineMotion : Traversable
	{
	private:
		geometry::Line m_line;

	public:
		explicit LineMotion(float startTime, float feedRate, const geometry::Line &line);

		QVector3D pointAtTime(float time) const;
	};

	class ArcMotion : Traversable
	{
	private:
		geometry::Arc m_arc;

	public:
		explicit ArcMotion(float startTime, float feedRate, const geometry::Arc &arc);

		QVector3D pointAtTime(float time) const;
	};

	using Motion = std::variant<LineMotion, ArcMotion>;

	using MotionList = std::vector<Motion>;
	MotionList m_motions;

	const Motion &findMotionAtTime(float time) const;

public:
	explicit Simulation(Document &document);

	QVector3D position(float time);
	float duration() const;
};

}

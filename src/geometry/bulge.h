#pragma once

#include <common/aggregable.h>

#include <geometry/arc.h>

#include <QVector2D>

namespace Geometry
{

class Bulge : public Common::Aggregable<Bulge>
{
private:
	QVector2D m_start;
	QVector2D m_end;

	float m_tangent;

public:
	explicit Bulge(const QVector2D &start, const QVector2D &end, float tangent);
	Bulge() = default;

	const QVector2D &start() const;
	const QVector2D &end() const;

	void invert();

	bool isLine() const;

	Arc toArc() const;
};

}

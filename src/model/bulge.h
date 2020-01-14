#pragma once

#include <QVector2D>

#include <vector>

namespace Model
{

class Bulge
{
private:
	QVector2D m_start;
	QVector2D m_end;

	float m_tangent;

public:
	explicit Bulge(const QVector2D &start, const QVector2D &end, float tangent);

	const QVector2D &start() const;
	const QVector2D &end() const;

	void invert();
};

using Bulges = std::vector<Bulge>;

}

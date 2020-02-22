#pragma once

#include <vector>
#include <optional>
#include <cmath>

#include <QVector2D>

namespace Geometry
{
	using Point2DList = std::vector<QVector2D>;

	inline float CrossProduct(const QVector2D &p0, const QVector2D &p1, const QVector2D &p2)
	{
		const QVector2D x = p1 - p0;
		const QVector2D y = p2 - p0;

		return x.x() * y.y() - y.x() * x.y();
	}

	inline std::optional<QVector2D> ForwardLineIntersection(const QVector2D &startA, const QVector2D &endA,
			const QVector2D &startB, const QVector2D &endB)
	{
		const float a1 = CrossProduct(startA, endA, startB);
		const float a2 = CrossProduct(startA, endA, endB);

		// Lines are parallel
		if (a1 == a2) {
			return {};
		}

		const float a3 = CrossProduct(startA, startB, endB);
		// Advance on first line
		const float t = a3 / (a2 - a1);

		// The intersection is backward the first line.
		if (t <= 0.0f) {
			return {};
		}

		return startA + (t * (endA - startA));
	}

	inline QVector2D TriangleIncenter(const QVector2D &t0, const QVector2D &t1, const QVector2D &t2)
	{
		const float a = (t1 - t2).length();
		const float b = (t0 - t2).length();
		const float c = (t0 - t1).length();

		const float sum = a + b + c;
		return (a * t0 + b * t1 + c * t2) / sum;
	}

	inline float NormalizedAngle(float angle)
	{
		return (angle < 0.0f) ? (angle + M_PI * 2.0f) : angle;
	}
}

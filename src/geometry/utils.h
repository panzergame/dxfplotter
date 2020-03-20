#pragma once

#include <vector>
#include <optional>
#include <cmath>
#include <cassert>

#include <QVector2D>

namespace Geometry
{
	using Point2DList = std::vector<QVector2D>;

	enum class Orientation
	{
		CW,
		CCW
	};

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
			return std::nullopt;
		}

		const float a3 = CrossProduct(startA, startB, endB);
		// Advance on first line
		const float t = a3 / (a2 - a1);

		// The intersection is backward the first line.
		if (t <= 0.0f) {
			return std::nullopt;
		}

		return std::make_optional(startA + (t * (endA - startA)));
	}

	inline QVector2D TriangleIncenter(const QVector2D &t0, const QVector2D &t1, const QVector2D &t2)
	{
		const float a = (t1 - t2).length();
		const float b = (t0 - t2).length();
		const float c = (t0 - t1).length();

		const float sum = a + b + c;
		return (a * t0 + b * t1 + c * t2) / sum;
	}

	inline QVector2D PerpendicularLine(const QVector2D &line)
	{
		return QVector2D(-line.y(), line.x());
	}

	inline QVector2D ReflectLine(const QVector2D &vector, const QVector2D &normal)
	{
		return vector - 2.0f * QVector2D::dotProduct(vector, normal) * normal;
	}

	inline float NormalizedAngle(float angle)
	{
		return (angle < 0.0f) ? (angle + M_PI * 2.0f) : angle;
	}

	inline float LineAngle(const QVector2D &line)
	{
		return std::atan2(line.y(), line.x());
	}

	/** Ensure start < end assuming start and end are engle of a CCW arc.
	 * @return new end angle, may remain unchanged
	 */
	inline float EnsureEndGreater(float start, float end)
	{
		if (end < start) {
			// Add pi*2 ensuring end is greater than start.
			end = M_PI * 2.0f + end;
		}

		assert(start < end);

		return end;
	}

	/// Return counter clockwise delta angle between start and end
	inline float DeltaAngle(float start, float end)
	{
		// End before start, meaning start is modulus PI*2
		if (end < start) {
			return (M_PI * 2.0f + end) - start;
		}
		return (end - start);
	}
}

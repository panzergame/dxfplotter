#pragma once

#define _USE_MATH_DEFINES // To include M_PI
#include <math.h>
#include <cmath>

#include <vector>
#include <optional>
#include <unordered_map>
#include <cassert>

#include <QVector2D>
#include <QVector3D>

#include <common/enum.h>

namespace geometry
{
	namespace precision
	{
		constexpr float fuzzyAngle = 1e-5f;
	}

	using Point2DList = std::vector<QVector2D>;
	using Point3DList = std::vector<QVector3D>;

	enum class Orientation
	{
		CW,
		CCW
	};

	enum class CuttingDirection
	{
		FORWARD,
		BACKWARD
	};

	inline CuttingDirection operator|(const CuttingDirection &dir1, const CuttingDirection &dir2)
	{
		return static_cast<CuttingDirection>((static_cast<int>(dir1) + static_cast<int>(dir2)) % 2);
	}

	inline float CrossProduct(const QVector2D &a, const QVector2D &b)
	{
		return a.x() * b.y() - b.x() * a.y();
	}

	inline float CrossProduct(const QVector2D &p0, const QVector2D &p1, const QVector2D &p2)
	{
		return CrossProduct(p1 - p0, p2 - p0);
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

	constexpr float NormalizedAngle(float angle)
	{
		return (angle < 0.0f) ? (angle + M_PI * 2.0f) : angle;
	}

	inline float LineAngle(const QVector2D &line)
	{
		return std::atan2(line.y(), line.x());
	}

	/** Ensure start < end assuming start and end are angle of a CCW arc.
	 * @return new end angle, may remain unchanged
	 */
	constexpr float EnsureEndGreater(float start, float end)
	{
		if (end < start) {
			// Add pi*2 ensuring end is greater than start.
			end += M_PI * 2.0f;
		}

		assert(start <= end);

		return end;
	}

	/// Return counter clockwise delta angle between start and end
	constexpr float DeltaAngle(float start, float end)
	{
		return (EnsureEndGreater(start, end) - start);
	}

	template <class Vector>
	constexpr Vector Lerp(const Vector& startPoint, const Vector& endPoint, float factor)
	{
		return startPoint * (1.0f - factor) + endPoint * factor;
	}
}

namespace common::enumerate
{

template <>
inline std::initializer_list<geometry::CuttingDirection> All()
{
	static const std::initializer_list<geometry::CuttingDirection> values = {geometry::CuttingDirection::FORWARD, geometry::CuttingDirection::BACKWARD};
	return values;
}

template <>
inline std::string toString(const geometry::CuttingDirection &direction)
{
	static const std::string translations[] = {
		"forward", // CuttingDirection::FORWARD
		"backward" // CuttingDirection::BACKWARD
	};

	return translations[static_cast<int>(direction)];
}

template <>
inline geometry::CuttingDirection fromString(const std::string &value)
{
	static const std::unordered_map<std::string, geometry::CuttingDirection> translations = {
		{"forward", geometry::CuttingDirection::FORWARD},
		{"backward", geometry::CuttingDirection::BACKWARD}
	};

	return translations.at(value);
}

}

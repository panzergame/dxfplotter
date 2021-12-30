#pragma once

#include <vector>
#include <optional>
#include <cmath>
#include <cassert>

#include <Eigen/Dense>

namespace geometry
{
	using Point2DList = std::vector<Eigen::Vector2d>;

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

	inline double CrossProduct(const Eigen::Vector2d &p0, const Eigen::Vector2d &p1, const Eigen::Vector2d &p2)
	{
		const Eigen::Vector2d x = p1 - p0;
		const Eigen::Vector2d y = p2 - p0;

		return x.x() * y.y() - y.x() * x.y();
	}

	inline std::optional<Eigen::Vector2d> ForwardLineIntersection(const Eigen::Vector2d &startA, const Eigen::Vector2d &endA,
			const Eigen::Vector2d &startB, const Eigen::Vector2d &endB)
	{
		const double a1 = CrossProduct(startA, endA, startB);
		const double a2 = CrossProduct(startA, endA, endB);

		// Lines are parallel
		if (a1 == a2) {
			return std::nullopt;
		}

		const double a3 = CrossProduct(startA, startB, endB);
		// Advance on first line
		const double t = a3 / (a2 - a1);

		// The intersection is backward the first line.
		if (t <= 0.0) {
			return std::nullopt;
		}

		return std::make_optional((startA + (t * (endA - startA))).eval());
	}

	inline Eigen::Vector2d TriangleIncenter(const Eigen::Vector2d &t0, const Eigen::Vector2d &t1, const Eigen::Vector2d &t2)
	{
		const double a = (t1 - t2).norm();
		const double b = (t0 - t2).norm();
		const double c = (t0 - t1).norm();

		const double sum = a + b + c;
		return (a * t0 + b * t1 + c * t2) / sum;
	}

	inline Eigen::Vector2d PerpendicularLine(const Eigen::Vector2d &line)
	{
		return {-line.y(), line.x()};
	}

	inline Eigen::Vector2d ReflectLine(const Eigen::Vector2d &vector, const Eigen::Vector2d &normal)
	{
		return vector - 2.0 * vector.dot(normal) * normal;
	}

	inline double NormalizedAngle(double angle)
	{
		return (angle < 0.0) ? (angle + M_PI * 2.0) : angle;
	}

	inline double LineAngle(const Eigen::Vector2d &line)
	{
		return std::atan2(line.y(), line.x());
	}

	/** Ensure start < end assuming start and end are angle of a CCW arc.
	 * @return new end angle, may remain unchanged
	 */
	inline double EnsureEndGreater(double start, double end)
	{
		if (end < start) {
			// Add pi*2 ensuring end is greater than start.
			end += M_PI * 2.0;
		}

		assert(start <= end);

		return end;
	}

	/// Return counter clockwise delta angle between start and end
	inline double DeltaAngle(double start, double end)
	{
		return (EnsureEndGreater(start, end) - start);
	}
}

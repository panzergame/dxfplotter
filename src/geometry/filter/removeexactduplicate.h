#pragma once

#include <geometry/polyline.h>

namespace geometry::filter
{

/** @brief Remove exact duplicate of polylines
 */
class RemoveExactDuplicate
{
private:
	Polyline::List m_polylines;

	struct UndirectedPolyline : common::Aggregable<UndirectedPolyline>
	{
		QVector2D start;
		QVector2D end;

		enum class Direction
		{
			Normal,
			Opposite
		} direction;
		int index;
		const Polyline *polyline;

		UndirectedPolyline() = default;
		explicit UndirectedPolyline(const QVector2D &start, const QVector2D &end, const Polyline &polyline, int index, Direction direction);

		bool operator<(const UndirectedPolyline &other) const;
		bool operator==(const UndirectedPolyline &other) const;
	};

	using PolylineMaskList = std::vector<bool>;

	UndirectedPolyline::List buildPolylinesMap(const Polyline::List& polylines) const;
	PolylineMaskList buildPolylineMaskWithoutDuplicate(const UndirectedPolyline::List polylineMap, int nbPolylines) const;
	void filterPolylines(Polyline::List &&polylines, PolylineMaskList polylineMask);

public:
	explicit RemoveExactDuplicate(Polyline::List &&polylines);

	Polyline::List &&polylines();
};

}

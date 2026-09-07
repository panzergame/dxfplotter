module;

#include <map>
#include <QVector2D>
#include <QDebug>

export module geometry.filter.removeexactduplicate;

import common.aggregable;
import geometry.bulge;
import geometry.polyline;
import geometry.utils;

export namespace geometry::filter
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

		enum class Direction { Normal, Opposite } direction;
		int index;
		const Polyline* polyline;

		UndirectedPolyline() = default;
		explicit UndirectedPolyline(const QVector2D& start, const QVector2D& end, const Polyline& polyline, int index,
									Direction direction);

		bool operator<(const UndirectedPolyline& other) const;
		bool operator==(const UndirectedPolyline& other) const;
	};

	using PolylineMaskList = std::vector<bool>;

	UndirectedPolyline::List buildPolylinesMap(const Polyline::List& polylines) const;
	PolylineMaskList buildPolylineMaskWithoutDuplicate(const UndirectedPolyline::List polylineMap,
													   int nbPolylines) const;
	void filterPolylines(Polyline::List&& polylines, PolylineMaskList polylineMask);

public:
	explicit RemoveExactDuplicate(Polyline::List&& polylines);

	Polyline::List&& polylines();
};

}

namespace geometry::filter
{

RemoveExactDuplicate::UndirectedPolyline::UndirectedPolyline(const QVector2D& start, const QVector2D& end,
															 const Polyline& polyline, int index, Direction direction)
	: start(start)
	, end(end)
	, direction(direction)
	, index(index)
	, polyline(&polyline)
{
}

bool operator<(const QVector2D& p1, const QVector2D& p2)
{
	if (p1.x() == p2.x()) {
		return p1.y() < p2.y();
	}
	return p1.x() < p2.x();
}

bool RemoveExactDuplicate::UndirectedPolyline::operator<(const UndirectedPolyline& other) const
{
	if (start == other.start) {
		return end < other.end;
	}
	return start < other.start;
}

bool RemoveExactDuplicate::UndirectedPolyline::operator==(const UndirectedPolyline& other) const
{
	if (index == other.index) {
		return false;
	}

	const bool sameTip = (start == other.start && end == other.end);
	if (!sameTip) {
		return false;
	}

	return polyline->equals(*other.polyline, direction == Direction::Opposite);
}

RemoveExactDuplicate::UndirectedPolyline::List
RemoveExactDuplicate::buildPolylinesMap(const Polyline::List& polylines) const
{
	const int nbPolylines = polylines.size();
	RemoveExactDuplicate::UndirectedPolyline::List polylinesMap;
	polylinesMap.reserve(nbPolylines * 2);

	for (int i = 0; i < nbPolylines; ++i) {
		const Polyline& polyline = polylines[i];
		if (!polyline.isPoint()) {
			const QVector2D& start = polyline.start();
			const QVector2D& end = polyline.end();

			polylinesMap.emplace_back(start, end, polyline, i, UndirectedPolyline::Direction::Normal);
			polylinesMap.emplace_back(end, start, polyline, i, UndirectedPolyline::Direction::Opposite);
		}
	}

	std::sort(polylinesMap.begin(), polylinesMap.end());

	return polylinesMap;
}

RemoveExactDuplicate::PolylineMaskList
RemoveExactDuplicate::buildPolylineMaskWithoutDuplicate(const UndirectedPolyline::List polylinesMap,
														int nbPolylines) const
{
	const int nbUndirectedPolylines = polylinesMap.size();
	PolylineMaskList mask(nbPolylines, true);

	for (int i = 0; i < (nbUndirectedPolylines - 1); ++i) {
		const UndirectedPolyline& undirectedPolyline = polylinesMap[i];
		const UndirectedPolyline& nextUndirectedPolyline = polylinesMap[i + 1];

		if (undirectedPolyline == nextUndirectedPolyline) {
			mask[undirectedPolyline.index] = false;
		}
	}

	return mask;
}

void RemoveExactDuplicate::filterPolylines(Polyline::List&& polylines, PolylineMaskList polylineMask)
{
	int removedPolylines = 0;
	for (int i = 0, size = polylines.size(); i < size; ++i) {
		if (polylineMask[i]) {
			m_polylines.emplace_back(std::move(polylines[i]));
		} else {
			++removedPolylines;
		}
	}

	qInfo() << "Removed" << removedPolylines << "duplicated polylines";
}

RemoveExactDuplicate::RemoveExactDuplicate(Polyline::List&& polylines)
{
	UndirectedPolyline::List polylinesMap = buildPolylinesMap(polylines);
	const int nbPolylines = polylines.size();
	PolylineMaskList polylineMask = buildPolylineMaskWithoutDuplicate(polylinesMap, nbPolylines);
	filterPolylines(std::move(polylines), polylineMask);
}

Polyline::List&& RemoveExactDuplicate::polylines()
{
	return std::move(m_polylines);
}

}

module;

#include <iterator>
#include <list>
#include <set>
#include <nanoflann.hpp>
#include <QVector2D>

export module geometry.filter.assembler;

import common.aggregable;
import geometry.polyline;
import geometry.utils;

export namespace geometry::filter
{

class Assembler
{
private:
	using PolylineIndex = int;
	using TipIndex = int;

	struct Tip : common::Aggregable<Tip>
	{
		PolylineIndex polylineIndex;
		// Original point from polyline.
		QVector2D point;

		enum class Type { START = 0, END } type;
	};

	static inline TipIndex tipIndexFromPolylineSide(PolylineIndex index, Tip::Type side)
	{
		return index * 2 + static_cast<int>(side);
	}

	class TipAdaptor
	{
	private:
		const Tip::List& m_tips;

	public:
		explicit TipAdaptor(const Tip::List& tips);

		size_t kdtree_get_point_count() const;
		float kdtree_get_pt(const size_t idx, const size_t dim) const;

		template<class BBOX>
		bool kdtree_get_bbox([[maybe_unused]] BBOX& bb) const
		{
			return false;
		}
	};

	using KDTree = nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L2_Adaptor<float, TipAdaptor>, TipAdaptor, 2>;

	class ChainBuilder
	{
	private:
		struct Item
		{
			PolylineIndex polylineIndex;
			enum class Direction {
				NORMAL = 0,
				INVERT ///< Inverted with previous polyline
			} dir;
		};

		using List = std::list<Item>;

		List m_chain;
		const Tip::List& m_tips;
		std::set<PolylineIndex>& m_unconnectedPolylines;
		const KDTree& m_tree;
		const PolylineIndex m_startIndex;
		const float m_closeTolerance;
		bool m_closed;

		template<class Inserter>
		bool expandSide(Inserter inserter, Tip::Type side)
		{
			// Direction of polyline, at first normal direction.
			Item::Direction direction = Item::Direction::NORMAL;

			PolylineIndex index = m_startIndex;
			while (index != -1) {
				const size_t tipIndex = tipIndexFromPolylineSide(index, side);
				// Tips of the current polyline at the right side.
				const Tip& tip = m_tips[tipIndex];

				// Coordinate of search point.
				const float coord[2] = { tip.point.x(), tip.point.y() };

				// Nearest neighbour with distance.
				std::array<std::uint32_t, 2> matchIndices;
				std::array<float, 2> matchDistances;

				// Search for the nearest neighbours.
				const int nbMatches = m_tree.knnSearch(coord, 2, matchIndices.data(), matchDistances.data());
				if (nbMatches == 2) {
					// Find neighbour that it's not ourself.
					const int neighbourMatchIndex = (matchIndices[0] == tipIndex) ? 1 : 0;
					const TipIndex neighbourTipIndex = matchIndices[neighbourMatchIndex];
					// Check if neighbour is not further than tolerance
					if (matchDistances[neighbourMatchIndex] <= m_closeTolerance) {
						const Tip& neighbour = m_tips[neighbourTipIndex];

						assert(&tip != &neighbour);

						const PolylineIndex neighbourIndex = neighbour.polylineIndex;
						// Stop when finding opposite tip of current polyline
						if (index == neighbourIndex) {
							return false;
						}
						/* If polyline is already connected (in case of circular shape
						 * one side can already connect all polylines) we discard.
						 */
						else if (m_unconnectedPolylines.find(neighbourIndex) == m_unconnectedPolylines.end()) {
							// The chain might be closed
							return true;
						} else {
							// If end matchs start then polylines are in same direction, otherwise they are opposed.
							const bool isOpposed = (tip.type == neighbour.type);
							Item::Direction neighbourDirection
								= static_cast<Item::Direction>((static_cast<int>(direction) + isOpposed) % 2);

							// Insert the polyline.
							*inserter = { neighbourIndex, neighbourDirection };

							// Remove polyline from unconnected list.
							m_unconnectedPolylines.erase(neighbourIndex);

							// Continue with the neighbour polyline.
							index = neighbourIndex;
							// Change to opposite side if polylines are opposed.
							side = static_cast<Tip::Type>((static_cast<int>(side) + isOpposed) % 2);
							// Update direction
							direction = neighbourDirection;
						}
					} else {
						// Neighbour too far
						index = -1;
					}
				} else {
					// None or too many neighbour
					index = -1;
				}
			}

			// The chain is fully expanded on one side without closing
			return false;
		}

	public:
		explicit ChainBuilder(const Tip::List& tips, std::set<PolylineIndex>& unconnectedPolylines, const KDTree& tree,
							  PolylineIndex index, float closeTolerance);

		Polyline mergedPolyline(const Polyline::List& polylines) const;
	};

	Polyline::List m_unmergedPolylines;
	const float m_closeTolerance;

	Polyline::List m_mergedPolylines;

	Tip::List constructTips();

	Polyline::List connectTips(const Tip::List& tips, const KDTree& tree) const;

public:
	explicit Assembler(Polyline::List&& polylines, float closeTolerance);

	Polyline::List&& polylines();
};

}

namespace geometry::filter
{

Assembler::ChainBuilder::ChainBuilder(const Tip::List& tips, std::set<PolylineIndex>& unconnectedPolylines,
									  const KDTree& tree, PolylineIndex index, float closeTolerance)
	: m_chain { { index, Item::Direction::NORMAL } }
	, m_tips(tips)
	, m_unconnectedPolylines(unconnectedPolylines)
	, m_tree(tree)
	, m_startIndex(index)
	, m_closeTolerance(closeTolerance)
	,
	// Expand chain before polyline
	m_closed(expandSide(std::front_inserter(m_chain), Tip::Type::START)
			 || expandSide(std::back_inserter(m_chain), Tip::Type::END))
{
}

/// Average point at p1 end and p2 start and assign middle point to both
static void averageStartEndPolyline(Polyline& first, Polyline& second)
{
	const QVector2D middlePoint = (first.end() + second.start()) / 2.0f;
	first.end() = middlePoint;
	second.start() = middlePoint;
}

Polyline Assembler::ChainBuilder::mergedPolyline(const Polyline::List& polylines) const
{
	// Initialise with first polyline
	const Item& firstItem = m_chain.front();
	Polyline mergedPolyline = polylines[firstItem.polylineIndex];
	if (firstItem.dir == Item::Direction::INVERT) {
		mergedPolyline.invert();
	}

	// Merge remaining polylines
	for (List::const_iterator it = ++m_chain.begin(), end = m_chain.end(); it != end; ++it) {
		Polyline polyline = polylines[it->polylineIndex];
		if (it->dir == Item::Direction::INVERT) {
			polyline.invert();
		}

		averageStartEndPolyline(mergedPolyline, polyline);
		mergedPolyline += polyline;
	}

	if (m_closed) {
		averageStartEndPolyline(mergedPolyline, mergedPolyline);
	}

	return mergedPolyline;
}

Assembler::TipAdaptor::TipAdaptor(const Tip::List& tips)
	: m_tips(tips)
{
}

size_t Assembler::TipAdaptor::kdtree_get_point_count() const
{
	return m_tips.size();
}

float Assembler::TipAdaptor::kdtree_get_pt(const size_t idx, const size_t dim) const
{
	return m_tips[idx].point[dim];
}

Assembler::Tip::List Assembler::constructTips()
{
	Tip::List tips; // TODO reserve and std::transform

	for (int i = 0, size = m_unmergedPolylines.size(); i < size; ++i) {
		const Polyline& polyline = m_unmergedPolylines[i];
		tips.push_back({ {}, i, polyline.start(), Tip::Type::START });
		tips.push_back({ {}, i, polyline.end(), Tip::Type::END });
	}

	return tips;
}

Polyline::List Assembler::connectTips(const Tip::List& tips, const KDTree& tree) const
{
	// Generate all unconnected polyline index.
	std::set<PolylineIndex> unconnectedPolylines;
	for (PolylineIndex index = 0, size = m_unmergedPolylines.size(); index < size; ++index) {
		unconnectedPolylines.insert(index);
	}

	Polyline::List mergedPolylines;

	while (!unconnectedPolylines.empty()) {
		// Pick a polyline index.
		const PolylineIndex index = *unconnectedPolylines.begin();
		unconnectedPolylines.erase(unconnectedPolylines.begin());

		ChainBuilder builder(tips, unconnectedPolylines, tree, index, m_closeTolerance);
		mergedPolylines.push_back(builder.mergedPolyline(m_unmergedPolylines));
	}

	return mergedPolylines;
}

Assembler::Assembler(Polyline::List&& polylines, float closeTolerance)
	: m_closeTolerance(closeTolerance)
{
	// Dispatch polylines to already merged or not merged.
	for (Polyline& polyline : polylines) {
		// Point polylines cannot be merged to others and so are ignored.
		if (polyline.isPoint()) {
			m_mergedPolylines.emplace_back(std::move(polyline));
		} else {
			m_unmergedPolylines.emplace_back(std::move(polyline));
		}
	}

	// Build tips based on unmerged polylines.
	const Tip::List tips = constructTips();

	// Build kdtree of tips for faster query.
	TipAdaptor adaptor(tips);
	KDTree tree(2, adaptor);
	tree.buildIndex();

	// Merge all unmerged polylines.
	const Polyline::List mergedPolylines = connectTips(tips, tree);
	// Concat all merged polylines.
	m_mergedPolylines.insert(m_mergedPolylines.end(), std::move_iterator(mergedPolylines.begin()),
							 std::move_iterator(mergedPolylines.end()));
}

Polyline::List&& Assembler::polylines()
{
	return std::move(m_mergedPolylines);
}

}

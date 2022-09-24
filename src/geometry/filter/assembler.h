#pragma once

#include <geometry/polyline.h>

#include <nanoflann.hpp>

#include <set>
#include <list>

namespace geometry::filter
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

		enum class Type {
			START = 0,
			END
		} type;
	};

	static inline TipIndex tipIndexFromPolylineSide(PolylineIndex index, Tip::Type side)
	{
		return index * 2 + static_cast<int>(side);
	}

	class TipAdaptor
	{
	private:
		const Tip::List &m_tips;

	public:
		explicit TipAdaptor(const Tip::List &tips);

		size_t kdtree_get_point_count() const;
		float kdtree_get_pt(const size_t idx, const size_t dim) const;

		template <class BBOX>
		bool kdtree_get_bbox([[maybe_unused]] BBOX &bb) const
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
			enum class Direction
			{
				NORMAL = 0,
				INVERT ///< Inverted with previous polyline
			} dir;
		};

		using List = std::list<Item>;

		List m_chain;
		const Tip::List &m_tips;
		std::set<PolylineIndex> &m_unconnectedPolylines;
		const KDTree &m_tree;
		const PolylineIndex m_startIndex;
		const float m_closeTolerance;
		bool m_closed;

		template <class Inserter>
		bool expandSide(Inserter inserter, Tip::Type side)
		{
			// Direction of polyline, at first normal direction.
			Item::Direction direction = Item::Direction::NORMAL;

			PolylineIndex index = m_startIndex;
			while (index != -1) {
				const size_t tipIndex = tipIndexFromPolylineSide(index, side);
				// Tips of the current polyline at the right side.
				const Tip &tip = m_tips[tipIndex];

				// Coordinate of search point.
				const float coord[2] = {tip.point.x(), tip.point.y()};

				// Nearest neighbour with distance.
				std::array<size_t, 2> matchIndices;
				std::array<float, 2> matchDistances;

				// Search for the nearest neighbours.
				const int nbMatches = m_tree.knnSearch(coord, 2, matchIndices.data(), matchDistances.data());
				if (nbMatches == 2) {
					// Find neighbour that it's not ourself.
					const int neighbourMatchIndex = (matchIndices[0] == tipIndex) ? 1 : 0;
					const TipIndex neighbourTipIndex = matchIndices[neighbourMatchIndex];
					// Check if neighbour is not further than tolerance
					if (matchDistances[neighbourMatchIndex] <= m_closeTolerance) {
						const Tip &neighbour = m_tips[neighbourTipIndex];

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
						}
						else {
							// If end matchs start then polylines are in same direction, otherwise they are opposed.
							const bool isOpposed = (tip.type == neighbour.type);
							Item::Direction neighbourDirection = static_cast<Item::Direction>((static_cast<int>(direction) + isOpposed) % 2);

							// Insert the polyline.
							*inserter = {neighbourIndex, neighbourDirection};

							// Remove polyline from unconnected list.
							m_unconnectedPolylines.erase(neighbourIndex);

							// Continue with the neighbour polyline.
							index = neighbourIndex;
							// Change to opposite side if polylines are opposed.
							side = static_cast<Tip::Type>((static_cast<int>(side) + isOpposed) % 2);
							// Update direction
							direction = neighbourDirection;
						}
					}
					else {
						// Neighbour too far
						index = -1;
					}
				}
				else {
					// None or too many neighbour
					index = -1;
				}
			}

			// The chain is fully expanded on one side without closing
			return false;
		}

	public:
		explicit ChainBuilder(const Tip::List &tips, std::set<PolylineIndex> &unconnectedPolylines, const KDTree &tree, PolylineIndex index, float closeTolerance);

		Polyline mergedPolyline(const Polyline::List &polylines) const;
	};

	Polyline::List m_unmergedPolylines;
	const float m_closeTolerance;

	Polyline::List m_mergedPolylines;

	Tip::List constructTips();

	Polyline::List connectTips(const Tip::List &tips, const KDTree &tree) const;

public:
	explicit Assembler(Polyline::List &&polylines, float closeTolerance);

	Polyline::List &&polylines();
};

}

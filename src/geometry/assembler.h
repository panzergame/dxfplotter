#pragma once

#include <geometry/polyline.h>

#include <nanoflann.hpp>

#include <set>

#include <QDebug>

namespace Geometry
{

class Assembler
{
private:
	using PolylineIndex = int;

	struct Tip : Common::Aggregable<Tip>
	{
		PolylineIndex polylineIndex;
		QVector2D point;

		enum Type {
			START = 0,
			END
		} type;
	};

	struct Item
	{
		PolylineIndex polylineIndex;
		enum Direction
		{
			NORMAL = 0,
			INVERT ///< Inverted with previous polyline
		} dir;
	};

	using Chain = std::list<Item>;

	class TipAdaptor
	{
	private:
		const Tip::List &m_tips;

	public:
		explicit TipAdaptor(const Tip::List &tips);

		size_t kdtree_get_point_count() const;
		float kdtree_get_pt(const size_t idx, const size_t dim) const;

		template <class BBOX>
		bool kdtree_get_bbox(BBOX &bb) const
		{
			return false;
		}
	};

	using KDTree = nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L2_Adaptor<float, TipAdaptor>, TipAdaptor, 2>;

	Polyline::List m_polylines;
	float m_closeTolerance;

	Polyline::List m_mergedPolylines;

	Tip::List constructTips();

	template <class Inserter>
	void expandChain(const Tip::List &tips, std::set<PolylineIndex> &unconnectedPolylines, const KDTree &tree, Inserter inserter, PolylineIndex index, Tip::Type side)
	{
		// Direction of polyline, at first normal direction.
		Item::Direction direction = Item::NORMAL;

		while (index != -1) {
			const size_t tipIndex = index * 2 + side;
			// Tips of the current polyline at the right side.
			const Tip &tip = tips[tipIndex];

			// Coordinate of search point.
			const float coord[2] = {tip.point.x(), tip.point.y()};

			// Nearest neighbour with distance.
			std::array<size_t, 2> matchIndices;
			std::array<float, 2> matchDistances;

			// Search for the nearest neighbours.
			const int nbMatches = tree.knnSearch(coord, 2, matchIndices.data(), matchDistances.data());

			if (nbMatches == 2) {
				// Find neighbour that it's not ourself.
				const int neighbourMatchIndex = (matchIndices[0] == tipIndex) ? 1 : 0;
				const int neighbourIndex = matchIndices[neighbourMatchIndex];
				// Check if neighbour is not further than tolerance
				if (matchDistances[neighbourMatchIndex] <= m_closeTolerance) {
					const Tip &neighbour = tips[neighbourIndex];

					assert(&tip != &neighbour);

					const PolylineIndex neighbourIndex = neighbour.polylineIndex;
					/* If polyline is already connected (in case of circular shape
					* one side can already connect all polylines) we discard.
					*/
					if (unconnectedPolylines.find(neighbourIndex) == unconnectedPolylines.end()) {
						index = -1;
					}
					else {
						// If end matchs start then polylines are in same direction, otherwise they are opposed.
						const bool opposed = (tip.type == neighbour.type);
						Item::Direction neighbourDirection = (Item::Direction)((direction + opposed) % 2);

						// Insert the polyline.
						*inserter++ = {neighbourIndex, neighbourDirection};

						// Remove polyline from unconnected list.
						unconnectedPolylines.erase(neighbourIndex);

						// Continue with the neighbour polyline.
						index = neighbourIndex;
						// Change to opposite side if polylines are opposed.
						side = (Tip::Type)((side + opposed) % 2);
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
				// No neighbour
				index = -1;
			}
		}
	}

	Polyline::List connectTips(const Tip::List &tips, const KDTree &tree);

public:
	explicit Assembler(Polyline::List &&polylines, float closeTolerance);

	Polyline::List &&mergedPolylines();
};

}

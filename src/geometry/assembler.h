#pragma once

#include <model/polyline.h>
#include <model/path.h>

#include <nanoflann.hpp>

#include <iostream>
#include <set>

namespace Geometry
{

class Assembler
{
private:
	using PolylineIndex = int;

	struct Tip
	{
		PolylineIndex polylineIndex;
		QVector2D point;

		enum Type {
			START = 0,
			END
		} type;
	};

	using Tips = std::vector<Tip>;

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
		const std::vector<Tip> &m_tips;

	public:
		explicit TipAdaptor(const std::vector<Tip> &tips);

		size_t kdtree_get_point_count() const;
		float kdtree_get_pt(const size_t idx, const size_t dim) const;

		template <class BBOX>
		bool kdtree_get_bbox(BBOX &bb) const
		{
			return false;
		}
	};

	using Connection = std::pair<const Tip *, const Tip *>;
	using Connections = std::vector<Connection>;

	using KDTree = nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L2_Adaptor<float, TipAdaptor>, TipAdaptor, 2>;

	Model::Polylines m_polylines;
	float m_closeTolerance;

	Model::Polylines m_mergedPolylines;

	Tips constructTips();

	template <class Inserter>
	void expandChain(const Tips &tips, std::set<PolylineIndex> &unconnectedPolylines, const KDTree &tree, Inserter inserter, PolylineIndex index, Tip::Type side)
	{
		// Direction of polyline, at first normal direction.
		Item::Direction direction = Item::NORMAL;

		while (index != -1) {
			// Tips of the current polyline at the right side.
			const Tip &tip = tips[index * 2 + side];

			// Coordinate of search point.
			const float coord[2] = {tip.point.x(), tip.point.y()};

			std::vector<std::pair<size_t, float> > matches;
			// Search for the nearest neighbours.
			const int nbMatches = tree.radiusSearch(coord, m_closeTolerance, matches, nanoflann::SearchParams());

			/* If there's more than 1 real neighbour (2 with it self included)
			* the shape is not a contour.
			*/
			if (nbMatches > 2) {
				std::cout << "not a countour" << std::endl; // TODO logging
				index = -1;
			}
			else if (nbMatches == 2) {
				// Find neighbour that it's not ourself.
				const Tip *neighbour;
				for (const auto &match : matches) {
					neighbour = &tips[match.first];
					if (neighbour != &tip) {
						break;
					}
				}

				assert(&tip != neighbour);

				const PolylineIndex neighbourIndex = neighbour->polylineIndex;
				/* If polyline is already connected (in case of circular shape
				 * one side can already connect all polylines) we discard.
				 */
				if (unconnectedPolylines.find(neighbourIndex) == unconnectedPolylines.end()) {
					index = -1;
				}
				else {
					// If end matchs start then polylines are in same direction, otherwise they are opposed.
					const bool opposed = (tip.type == neighbour->type);
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
				index = -1;
			}
		}
	}

	Model::Polylines connectTips(const Tips &tips, const KDTree &tree);

public:
	explicit Assembler(Model::Polylines &&polylines, float closeTolerance);

	Model::Polylines &&mergedPolylines();
};

}

#include <assembler.h>

#include <iterator>

namespace Geometry
{

Assembler::TipAdaptor::TipAdaptor(const Tip::List &tips)
	:m_tips(tips)
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

	for (int i = 0, size = m_polylines.size(); i < size; ++i) {
		const Polyline &polyline = m_polylines[i];
		tips.push_back({{}, i, polyline.start(), Tip::START});
		tips.push_back({{}, i, polyline.end(), Tip::END});
	}

	return tips;
}

Polyline::List Assembler::connectTips(const Tip::List &tips, const KDTree &tree)
{
	std::set<PolylineIndex> unconnectedPolylines;

	// Generate all unconnected polyline index.
	for (int i = 0, size = m_polylines.size(); i < size; ++i) {
		unconnectedPolylines.insert(i);
	}

	Polyline::List mergedPolylines;

	while (!unconnectedPolylines.empty()) {
		// Retrieve a polyline.
		PolylineIndex index = *unconnectedPolylines.begin();
		unconnectedPolylines.erase(unconnectedPolylines.begin());

		// Initialise a single polyline chain.
		Chain chain{{index, Item::NORMAL}};

		// Expand chain before polyline
		expandChain(tips, unconnectedPolylines, tree, std::front_inserter(chain), index, Tip::START);
		// Expand chain after polyline
		expandChain(tips, unconnectedPolylines, tree, std::back_inserter(chain), index, Tip::END);

		Polyline mergedPolyline;
		// Build a single merged polyline.
		for (const Item &item : chain) {
			Polyline &polyline = m_polylines[item.polylineIndex];
			if (item.dir == Item::INVERT) {
				polyline.invert();
			}

			mergedPolyline += polyline;
		}
		mergedPolylines.push_back(mergedPolyline);
	}

	return mergedPolylines;
}

Assembler::Assembler(Polyline::List &&polylines, float closeTolerance)
	:m_polylines(polylines),
	m_closeTolerance(closeTolerance)
{
	const Tip::List tips = constructTips(); // TODO move all in member variable and no arguments

	TipAdaptor adaptor(tips);
	KDTree tree(2, adaptor);
	tree.buildIndex();

	m_mergedPolylines = connectTips(tips, tree);
}

Polyline::List &&Assembler::polylines()
{
	return std::move(m_mergedPolylines);
}

}

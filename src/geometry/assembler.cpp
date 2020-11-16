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

/// Merge point at p1 end and p2 start and assign middle point to both
static void mergePolylineStartEnd(Polyline &first, Polyline &second)
{
	const QVector2D middlePoint = (first.end() + second.start()) / 2.0f;
	first.end() = middlePoint;
	second.start() = middlePoint;
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
		bool closed = expandChain(tips, unconnectedPolylines, tree, std::front_inserter(chain), index, Tip::START);
		// Expand chain after polyline
		if (!closed) {
			closed = expandChain(tips, unconnectedPolylines, tree, std::back_inserter(chain), index, Tip::END);
		}

		// Initialise with first chain polyline
		Polyline mergedPolyline = m_polylines[chain.front().polylineIndex];
		// Merge remaining polylines
		for (Chain::const_iterator it = ++chain.begin(), end = chain.end(); it != end; ++it) {
			Polyline &polyline = m_polylines[it->polylineIndex];
			if (it->dir == Item::INVERT) {
				polyline.invert();
			}

			mergePolylineStartEnd(mergedPolyline, polyline);
			mergedPolyline += polyline;
		}

		if (closed) {
			mergePolylineStartEnd(mergedPolyline, mergedPolyline);
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

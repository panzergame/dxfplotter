#include <assembler.h>

#include <iterator>

namespace Geometry
{

Assembler::ChainBuilder::ChainBuilder(const Tip::List &tips, std::set<PolylineIndex> &unconnectedPolylines, const KDTree &tree, PolylineIndex index, float closeTolerance)
	:m_chain{{index, Item::NORMAL}},
	m_closed(false),
	m_tips(tips),
	m_unconnectedPolylines(unconnectedPolylines),
	m_tree(tree),
	m_startIndex(index),
	m_closeTolerance(closeTolerance)
{
	// Expand chain before polyline
	m_closed = expandSide(std::front_inserter(m_chain), Tip::START);
	// Expand chain after polyline
	if (!m_closed) {
		m_closed = expandSide(std::back_inserter(m_chain), Tip::END);
	}
}

Assembler::TipIndex Assembler::ChainBuilder::tipIndexFromPolylineSide(PolylineIndex index, Tip::Type side)
{
	return index * 2 + side;
}

/// Average point at p1 end and p2 start and assign middle point to both
static void averageStartEndPolyline(Polyline &first, Polyline &second)
{
	const QVector2D middlePoint = (first.end() + second.start()) / 2.0f;
	first.end() = middlePoint;
	second.start() = middlePoint;
}

Polyline Assembler::ChainBuilder::mergedPolyline(const Polyline::List& polylines) const
{
	// Initialise with first polyline
	const Item &firstItem = m_chain.front();
	Polyline mergedPolyline = polylines[firstItem.polylineIndex];
	if (firstItem.dir == Item::INVERT) {
		mergedPolyline.invert();
	}

	// Merge remaining polylines
	for (List::const_iterator it = ++m_chain.begin(), end = m_chain.end(); it != end; ++it) {
		Polyline polyline = polylines[it->polylineIndex];
		if (it->dir == Item::INVERT) {
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
	// Generate all unconnected polyline index.
	std::set<PolylineIndex> unconnectedPolylines;
	std::iota(unconnectedPolylines.begin(), unconnectedPolylines.end(), 0);

	Polyline::List mergedPolylines;

	while (!unconnectedPolylines.empty()) {
		// Pick a polyline index.
		const PolylineIndex index = *unconnectedPolylines.begin();
		unconnectedPolylines.erase(unconnectedPolylines.begin());

		ChainBuilder builder(tips, unconnectedPolylines, tree, index, m_closeTolerance);
		mergedPolylines.push_back(builder.mergedPolyline(m_polylines));
	}

	return mergedPolylines;
}

Assembler::Assembler(Polyline::List &&polylines, float closeTolerance)
	:m_polylines(polylines),
	m_closeTolerance(closeTolerance)
{
	const Tip::List tips = constructTips();

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

#include <assembler.h>

#include <iterator>

namespace geometry::filter
{

Assembler::ChainBuilder::ChainBuilder(const Tip::List &tips, std::set<PolylineIndex> &unconnectedPolylines, const KDTree &tree, PolylineIndex index, float closeTolerance)
	:m_chain{{index, Item::Direction::NORMAL}},
	m_tips(tips),
	m_unconnectedPolylines(unconnectedPolylines),
	m_tree(tree),
	m_startIndex(index),
	m_closeTolerance(closeTolerance),
	// Expand chain before polyline
	m_closed(expandSide(std::front_inserter(m_chain), Tip::Type::START) || expandSide(std::back_inserter(m_chain), Tip::Type::END))
{
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

	for (int i = 0, size = m_unmergedPolylines.size(); i < size; ++i) {
		const Polyline &polyline = m_unmergedPolylines[i];
		tips.push_back({{}, i, polyline.start(), Tip::Type::START});
		tips.push_back({{}, i, polyline.end(), Tip::Type::END});
	}

	return tips;
}

Polyline::List Assembler::connectTips(const Tip::List &tips, const KDTree &tree) const
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

Assembler::Assembler(Polyline::List &&polylines, float closeTolerance)
	:m_closeTolerance(closeTolerance)
{
	// Dispatch polylines to already merged or not merged.
	for (Polyline& polyline : std::move(polylines)) {
		// Point polylines cannot be merged to others and so are ignored.
		if (polyline.isPoint()) {
			m_mergedPolylines.emplace_back(std::move(polyline));
		}
		else {
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
    m_mergedPolylines.insert(m_mergedPolylines.end(), std::move_iterator(mergedPolylines.begin()), std::move_iterator(mergedPolylines.end()));
}

Polyline::List &&Assembler::polylines()
{
	return std::move(m_mergedPolylines);
}

}

#include <cleaner.h>

#include <iterator>

#include <QDebug> // TODO

#include <taskflow/taskflow.hpp>
#include <taskflow/algorithm/transform.hpp>

namespace geometry
{

template<typename T>
typename std::vector<T>::iterator insertSortedReverse(std::vector<T> &vec, const T &item)
{
    return vec.insert(std::upper_bound(vec.begin(), vec.end(), item, std::greater<T>()), item);
}

class PolylineLengthCleaner
{
private:
	using BulgeLinkedList = std::list<Bulge>;

	struct Item : common::Aggregable<Item>
	{
		BulgeLinkedList::iterator it;
		float length;

		explicit Item(BulgeLinkedList::iterator _it, float _length)
			:it(_it),
			length(_length)
		{
		}

		bool operator>(const Item &other) const
		{
			return length > other.length;
		}
	};

	const Polyline &m_polyline;
	const float m_minimumPolylineLength;

	BulgeLinkedList m_bulges;
	Item::List m_itemsToMerge;

	void constructLinkedList()
	{
		m_polyline.forEachBulge([this](const Bulge &bulge){
			m_bulges.push_back(bulge);
		});
	}

	void initBulgesToMerge()
	{
		// Add every small bulges
		for (BulgeLinkedList::iterator it = m_bulges.begin(), end = m_bulges.end(); it != end; ++it) {
			const float length = it->length();
			if (length < m_minimumPolylineLength) {
				m_itemsToMerge.emplace_back(it, length);
			}
		}

		// Sort small bulges by length in descending order
		std::sort(m_itemsToMerge.begin(), m_itemsToMerge.end(), std::greater<Item>());
	}

	BulgeLinkedList::iterator extendNeighbourBulge(const BulgeLinkedList::iterator &it)
	{
		BulgeLinkedList::iterator neighbourIt;

		// Find neighbour bulge and extend it to overlap removed bulge.
		if (it == m_bulges.begin()) {
			neighbourIt = std::next(it);
			// Replace neighbour bulge with its extended version
			*neighbourIt = neighbourIt->extendStart(it->start());
		}
		else {
			neighbourIt = std::prev(it);
			// Replace neighbour bulge with its extended version
			*neighbourIt = neighbourIt->extendEnd(it->end());
		}

		return neighbourIt;
	}

	void removeAssociatedItem(const BulgeLinkedList::iterator &it)
	{
		// Look for item associated to neighbour bulge
		Item::List::iterator itemIt = std::find_if(m_itemsToMerge.begin(), m_itemsToMerge.end(),
				[&it](const Item &item) { return item.it == it; });

		// Remove this item.
		if (itemIt != m_itemsToMerge.end()) {
			m_itemsToMerge.erase(itemIt);
		}
	}

	void mergeItem(const Item &item)
	{
		const BulgeLinkedList::iterator &it = item.it;
		// Extend neighbour to overlap and retrieve its position
		const BulgeLinkedList::iterator neighbourIt = extendNeighbourBulge(it);

		// Remove smallest bulge.
		m_bulges.erase(it);

		// Try remove associated item to neighbour bulge
		removeAssociatedItem(neighbourIt);

		const float length = neighbourIt->length();
		// Reinsert extended bulge if still need to be merged.
		if (length < m_minimumPolylineLength) {
			insertSortedReverse(m_itemsToMerge, Item(neighbourIt, length));
		}
	}

public:
	explicit PolylineLengthCleaner(const Polyline &polyline, float minimumPolylineLength)
		:m_polyline(polyline),
		m_minimumPolylineLength(minimumPolylineLength)
	{
		constructLinkedList();

		initBulgesToMerge();

		// Merge all bulges but avoid emptying the polyline, for instance polylines representing a point
		while (!m_itemsToMerge.empty() && m_bulges.size() > 1) {
			Item item = m_itemsToMerge.back();
			m_itemsToMerge.pop_back();

			mergeItem(item);
		}
	}

	Polyline polyline() const
	{
		return Polyline(Bulge::List(m_bulges.begin(), m_bulges.end()));
	}
};

class ArcLengthCleaner
{
private:
	Polyline m_polyline;

public:
	explicit ArcLengthCleaner(Polyline &&polyline, float minimumArcLength)
		:m_polyline(polyline)
	{
		m_polyline.transformBulge([minimumArcLength](Bulge &bulge){
			if (bulge.isArc() && bulge.length() < minimumArcLength) {
				bulge.linify();
			}
		});
	}

	Polyline &&polyline()
	{
		return std::move(m_polyline);
	}
};

Cleaner::Cleaner(Polyline::List &&polylines, float minimumPolylineLength, float minimumArcLength)
{
	const std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

	m_polylines.resize(polylines.size());

#if 0
	std::transform(polylines.begin(), polylines.end(), m_polylines.begin(),
		[minimumPolylineLength, minimumArcLength](const Polyline &polyline) {
			// Prune small polyline length
			PolylineLengthCleaner lengthCleaner(polyline, minimumPolylineLength);

			// Convert small arcs to lines
			ArcLengthCleaner arcCleaner(lengthCleaner.polyline(), minimumArcLength);

			return arcCleaner.polyline();
		});
#else

	tf::Executor executor;
	tf::Taskflow taskflow;

	taskflow.transform(polylines.begin(), polylines.end(), m_polylines.begin(),
		[minimumPolylineLength, minimumArcLength](const Polyline &polyline) {
			// Prune small polyline length
			PolylineLengthCleaner lengthCleaner(polyline, minimumPolylineLength);

			// Convert small arcs to lines
			ArcLengthCleaner arcCleaner(lengthCleaner.polyline(), minimumArcLength);

			return arcCleaner.polyline();
		});

	executor.run(taskflow);
	executor.wait_for_all();
#endif

	const std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

	qInfo() << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

Polyline::List &&Cleaner::polylines()
{
	return std::move(m_polylines);
}

}

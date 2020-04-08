#include <cleaner.h>

#include <iterator>

namespace Geometry
{

template<typename T>
typename std::vector<T>::iterator insert_sorted(std::vector<T> &vec, const T &item)
{
    return vec.insert(std::upper_bound(vec.begin(), vec.end(), item), item);
}

class PolylineCleaner
{
private:
	using BulgeLinkedList = std::list<Bulge>;

	struct Item : Common::Aggregable<Item>
	{
		BulgeLinkedList::iterator it;
		float length;

		explicit Item(BulgeLinkedList::iterator _it, float _length)
			:it(_it),
			length(_length)
		{
		}

		bool operator<(const Item &other) const
		{
			return length < other.length;
		}
	};

	const Polyline &m_polyline;
	const float m_mergeTolerance;

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
		for (BulgeLinkedList::iterator it = m_bulges.begin(), end = m_bulges.end(); it != end; ++it) {
			const float length = it->length();
			if (length < m_mergeTolerance) {
				m_itemsToMerge.emplace_back(it, length);
			}
		}

		std::sort(m_itemsToMerge.begin(), m_itemsToMerge.end());
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
		if (length < m_mergeTolerance) {
			insert_sorted(m_itemsToMerge, Item(neighbourIt, length));
		}
	}

public:
	explicit PolylineCleaner(const Polyline &polyline, float mergeTolerance)
		:m_polyline(polyline),
		m_mergeTolerance(mergeTolerance)
	{
		constructLinkedList();

		initBulgesToMerge();

		while (!m_itemsToMerge.empty()) {
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

Cleaner::Cleaner(Polyline::List &&polylines, float mergeTolerance)
	:m_polylines(polylines.size()),
	m_mergeTolerance(mergeTolerance)
{
	std::transform(polylines.begin(), polylines.end(), m_polylines.begin(),
		[mergeTolerance = m_mergeTolerance](const Polyline &polyline)
		{
			PolylineCleaner cleaner(polyline, mergeTolerance);
			return cleaner.polyline();
		});
}

Polyline::List &&Cleaner::polylines()
{
	return std::move(m_polylines);
}

}

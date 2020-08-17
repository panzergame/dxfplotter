#pragma once

#include <variant>
#include <vector>
#include <map>
#include <memory>

#include <config/node.h>
#include <config/section.h>

namespace Config
{

/** @brief A configuration node list.
 * Node list contains children nodes and is named.
 */
class NodeList : public Node // TODO .cpp
{
	using Node::Node;

protected:
	using Child = std::unique_ptr<Node>;

	std::vector<Child> m_children;
	std::map<std::string, int> m_nameToIndex;

	/// Update name to child indices association
	void updateNameToIndexMap()
	{
		for (int i = 0, size = m_children.size(); i < size; ++i) {
			const Child &item = m_children[i];
			m_nameToIndex[item->name()] = i;
		}
	}

	template <class Item, class Index>
	Item &at(const Index &id)
	{
		return static_cast<Item &>(this->operator[](id));
	}

	template <class Item, class Index>
	const Item &at(const Index &id) const
	{
		return static_cast<const Item &>(this->operator[](id));
	}

public:
	Node &operator[](int index)
	{
		return *m_children[index];
	}

	const Node &operator[](int index) const
	{
		return *m_children[index];
	}

	Node &operator[](const std::string &name)
	{
		return *m_children[m_nameToIndex.at(name)];
	}

	const Node &operator[](const std::string &name) const
	{
		return *m_children[m_nameToIndex.at(name)];
	}

	bool has(const std::string &name) const
	{
		return m_nameToIndex.find(name) != m_nameToIndex.end();
	}

	int size() const
	{
		return m_children.size();
	}

	template <class Visitor>
	void visitChildren(Visitor &&visitor)
	{
		for (Child &child : m_children) {
			visitor(*child.get());
		}
	}
};

}

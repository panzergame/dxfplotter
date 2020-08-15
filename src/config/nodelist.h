#pragma once

#include <variant>
#include <vector>
#include <map>

#include <config/node.h>
#include <config/section.h>

namespace Config
{

/** @brief A configuration node list.
 * Node list contains children nodes and is named.
 */
template <class Item>
class NodeList : public Node
{
	using Node::Node;

protected:
	std::vector<Item> m_children;
	std::map<std::string, int> m_nameToIndex;

	/// Update name to child indices association
	void updateNameToIndexMap()
	{
		for (int i = 0, size = m_children.size(); i < size; ++i) {
			const Node &item = m_children[i];
			m_nameToIndex[item.name()] = i;
		}
	}

public:
	Item &operator[](int index)
	{
		return m_children[index];
	}

	const Item &operator[](int index) const
	{
		return m_children[index];
	}

	Item &operator[](const std::string &name)
	{
		return m_children[m_nameToIndex.at(name)];
	}

	const Item &operator[](const std::string &name) const
	{
		return m_children[m_nameToIndex.at(name)];
	}

	bool has(const std::string &name) const
	{
		return m_nameToIndex.find(name) != m_nameToIndex.end();
	}

	template <class Visitor>
	void visitChildren(Visitor &&visitor)
	{
		for (Item &child : m_children) {
			std::visit(visitor, child);
		}
	}
};

}

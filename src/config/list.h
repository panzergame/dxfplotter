#pragma once

#include <vector>
#include <unordered_map>

#include <config/node.h>

namespace Config
{

/** @brief A configuration node list.
 * Node list contains child nodes and is named.
 */
template <class Child>
class List : public Node
{
private:
	std::vector<Child> m_children;
	std::unordered_map<std::string, int> m_nameToIndex;

	/// Associate each ame to an item index for name lookup
	void updateNameToIndex()
	{
		for (int i = 0, size = m_children.size(); i < size; ++i) {
			const Child &child = m_children[i];

			m_nameToIndex[child.name()] = i;
		}
	}

public:
	explicit List(const std::string& name, const std::string &description, YAML::Node yamlNode)
		:Node(name, description)
	{
		// Creating by default one item
		if (!yamlNode.IsDefined()) {
			m_children.emplace_back("default", yamlNode["default"]);
		}
		else {
			// Create items from yaml node
			for (std::pair<YAML::Node, YAML::Node> pair : yamlNode) {
				m_children.emplace_back(pair.first.as<std::string>(), pair.second);
			}
		}

		updateNameToIndex();
	}

	List() = default;

	bool has(const std::string &name) const
	{
		return m_nameToIndex.find(name) != m_nameToIndex.end();
	}

	Child &operator[](const std::string &name)
	{
		return m_children[m_nameToIndex.at(name)];
	}

	const Child &operator[](const std::string &name) const
	{
		return m_children[m_nameToIndex.at(name)];
	}

	Child &operator[](int index)
	{
		return m_children[index];
	}

	const Child &operator[](int index) const
	{
		return m_children[index];
	}

	template <class Visitor>
	void visitChildren(Visitor &&visitor)
	{
		for (Child &child : m_children) {
			visitor(child);
		}
	}

		template <class Visitor>
	void visitChildren(Visitor &&visitor) const
	{
		for (const Child &child : m_children) {
			visitor(child);
		}
	}
};

}

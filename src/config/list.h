#pragma once

#include <vector>
#include <map>

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
	std::map<std::string, Child> m_children;
	YAML::Node m_yamlNode;

public:
	explicit List(const std::string& name, const std::string &description, YAML::Node yamlNode)
		:Node(name, description),
		m_yamlNode(yamlNode)
	{
		// Creating by default one item
		if (!m_yamlNode.IsDefined()) {
			m_children.emplace("default", Child("default", m_yamlNode["default"]));
		}
		else {
			// Create items from yaml node
			for (std::pair<YAML::Node, YAML::Node> pair : m_yamlNode) {
				const std::string &name = pair.first.as<std::string>();
				m_children.emplace(name, Child(name, pair.second));
			}
		}
	}

	List() = default;

	bool has(const std::string &name) const
	{
		return m_children.find(name) != m_children.end();
	}

	Child &operator[](const std::string &name)
	{
		return m_children.find(name)->second;
	}

	const Child &operator[](const std::string &name) const
	{
		return m_children.find(name)->second;
	}

	Child &first()
	{
		return m_children.begin()->second;
	}

	const Child &first() const
	{
		return m_children.begin()->second;
	}

	template <class Visitor>
	void visitChildren(Visitor &&visitor)
	{
		for (auto &pair : m_children) {
			visitor(pair.second);
		}
	}

		template <class Visitor>
	void visitChildren(Visitor &&visitor) const
	{
		for (auto &pair : m_children) {
			visitor(pair.second);
		}
	}

	/// Create a new named children
	Child &createChild(const std::string &name)
	{
		// TODO ensure name is unique
		const auto &pair = m_children.emplace(name, Child(name, m_yamlNode[name]));

		return pair.first->second;
	}

	void removeChild(const Child &child)
	{
		const std::string &name = child.name();

		m_yamlNode.remove(name);
		m_children.erase(name);
	}
};

}

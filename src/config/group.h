#pragma once

#include <tuple>

#include <config/node.h>

namespace Config
{

/** @brief A configuration group.
 */
template <class ... ChildTypes>
class Group : public Node
{
protected:
	std::tuple<ChildTypes ...> m_children;

public:
	explicit Group(const std::string& name, const std::string &description)
		:Node(name, description)
	{
	}

	Group() = default;

	template <class Visitor>
	void visitChildren(Visitor &&visitor)
	{
		std::apply([&visitor](auto&&... arg) {((visitor(arg)), ...);}, m_children);
	}

	template <class Visitor>
	void visitChildren(Visitor &&visitor) const
	{
		std::apply([&visitor](auto&&... arg) {((visitor(arg)), ...);}, m_children);
	}
};

}

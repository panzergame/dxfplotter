#pragma once

#include <tuple>

#include <config/node.h>

namespace config
{

/** @brief A configuration group.
 */
template <class ... ChildTypes>
class Group : public Node
{
protected:
	using Children = std::tuple<ChildTypes ...>;

	const Children& children() const
	{
		return m_children;
	}

	Children& children()
	{
		return m_children;
	}

	void setChildren(const Children& children)
	{
		m_children = children;
	}
	
private:
	Children m_children;

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

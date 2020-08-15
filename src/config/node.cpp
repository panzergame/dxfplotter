#include <node.h>
#include <algorithm>

namespace Config
{

static std::string titled(const std::string &input)
{
	std::string result(input.size(), '\0');
	// Remove all -
	std::replace_copy(input.begin(), input.end(), result.begin(), '-', ' ');
	// Capitalize first letter
	result.front() = std::toupper(result.front());

	return result;
}

Node::Node(const std::string &name)
	:m_name(name),
	m_description(titled(name))
{
}

const std::string &Node::name() const
{
	return m_name;
}

const std::string &Node::description() const
{
	return m_description;
}

}

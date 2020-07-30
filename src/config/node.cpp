#include <node.h>
#include <algorithm>

namespace Config
{

static std::string snakeCase(const std::string &input)
{
	std::string result(input.size(), '\0');
	std::replace_copy(input.begin(), input.end(), result.begin(), ' ', '_');

	return result;
}

static std::string titled(const std::string &input)
{
	std::string result = input;
	result.front() = std::toupper(result.front());

	return result;
}

Node::Node(const std::string &name)
	:m_name(snakeCase(name)),
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

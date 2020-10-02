#include <node.h>
#include <algorithm>

namespace Config
{

Node::Node(const std::string &name, const std::string &description)
	:m_name(name),
	m_description(description)
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

module;

#include <algorithm>
#include <string>
#include <yaml-cpp/yaml.h>

export module config.node;

export namespace config
{

class Node
{
private:
	std::string m_name;
	std::string m_description;

public:
	explicit Node(const std::string& name, const std::string& description)
		: m_name(name)
		, m_description(description)
	{
	}

	Node() = default;

	const std::string& name() const { return m_name; }

	const std::string& description() const { return m_description; }
};

}

#pragma once

#include <string>

#include <yaml-cpp/yaml.h>

namespace config
{

class Node
{
protected:
	std::string m_name;
	std::string m_description;

public:
	explicit Node(const std::string &name, const std::string &description);
	Node() = default;

	const std::string &name() const;
	const std::string &description() const;
};

}

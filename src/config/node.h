#pragma once

#include <string>

namespace Config
{

class Node
{
protected:
	std::string m_name;
	std::string m_description;

public:
	explicit Node() = default;
	explicit Node(const std::string &name);

	const std::string &name() const;
	const std::string &description() const;
};

}

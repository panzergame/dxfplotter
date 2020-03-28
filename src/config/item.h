#pragma once

#include <string>

namespace Config
{

class Item
{
protected:
	const std::string m_name;
	const std::string m_description;

public:
	explicit Item(const std::string& name, const std::string &description);

	const std::string &name() const;
	const std::string &description() const;
};

}

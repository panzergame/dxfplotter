#include <item.h>

namespace Config
{

Item::Item(const std::string &name, const std::string &description)
	:m_name(name),
	m_description(description)
{
}

const std::string &Item::name() const
{
	return m_name;
}

const std::string &Item::description() const
{
	return m_description;
}

}

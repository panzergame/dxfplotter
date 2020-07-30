#pragma once

#include <variant>
#include <vector>
#include <map>

#include <tinyxml2.h>

#include <config/section.h>

namespace Config
{

/** @brief A configuration group.
 * Group contains either sections or groups.
 */
class Group : public NodeList<std::variant<Group, Section>>
{
private:
	template <class Item>
	Item &at(const std::string &name)
	{
		return std::get<Item>(NodeList::operator[](name));
	}

	template <class Item>
	const Item &at(const std::string &name) const
	{
		return std::get<Item>(NodeList::operator[](name));
	}

	/// Update name to child indices association
	void updateNameToIndexMap();

public:
	explicit Group() = default;
	explicit Group(tinyxml2::XMLElement *root);

	Section &section(const std::string &name);
	const Section &section(const std::string &name) const;
	Group &group(const std::string &name);
	const Group &group(const std::string &name) const;
};

}

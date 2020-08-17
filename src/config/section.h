#pragma once

#include <variant>

#include <tinyxml2.h>

#include <config/nodelist.h>
#include <config/variable.h>

namespace Config
{

/** @brief A configuration section.
 * Section contains properties.
 */
class Section : public NodeList
{
private:
	void addVariable(tinyxml2::XMLElement *elem, YAML::Node &section);

public:
	explicit Section(tinyxml2::XMLElement *root, YAML::Node &section);

	template <class Index>
	Variable &var(const Index &id)
	{
		return at<Variable>(id);
	}

	template <class Index>
	const Variable &var(const Index &id) const
	{
		return at<Variable>(id);
	}
};

}

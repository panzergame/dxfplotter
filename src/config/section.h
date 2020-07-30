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
class Section : public NodeList<Variable>
{
private:
	void addVariable(tinyxml2::XMLElement *elem);

public:
	explicit Section(tinyxml2::XMLElement *root);
};

}

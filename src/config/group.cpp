#include <group.h>

#include <QDebug> // TODO

namespace Config
{

Group::Group(tinyxml2::XMLElement *root, YAML::Node &section)
	:NodeList(root->Attribute("name"))
{
	// Creating children depending on their type
	tinyxml2::XMLElement *child = root->FirstChildElement();
	while (child) {
		// Child type name e.g section, group
		const char *type = child->Name();
		// Child name
		const char *name = child->Attribute("name");
		YAML::Node childSection = section[name];

		if (strcmp(type, "group") == 0) {
			m_children.emplace_back(new Group(child, childSection));
		}
		else if (strcmp(type, "section") == 0) {
			m_children.emplace_back(new Section(child, childSection));
		}

		child = child->NextSiblingElement();
	}

	updateNameToIndexMap();
}

}

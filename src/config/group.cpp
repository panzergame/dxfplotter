#include <group.h>

#include <QDebug> // TODO

namespace Config
{

void Group::updateNameToIndexMap()
{
	for (int i = 0, size = m_children.size(); i < size; ++i) {
		std::visit([&nameToIndex=m_nameToIndex, i](auto &&item){
			nameToIndex[item.name()] = i;
			
		}, m_children[i]);
	}
}

Group::Group(tinyxml2::XMLElement *root)
	:NodeList(root->Attribute("name"))
{
	// Creating children depending on their type
	tinyxml2::XMLElement *child = root->FirstChildElement();
	while (child) {
		const char *name = child->Name();
		if (strcmp(name, "group") == 0) {
			m_children.emplace_back(Group(child));
		}
		else if (strcmp(name, "section") == 0) {
			m_children.emplace_back(Section(child));
		}

		child = child->NextSiblingElement();
	}

	updateNameToIndexMap();
}

Section &Group::section(const std::string &name)
{
	return at<Section>(name);
}

const Section &Group::section(const std::string &name) const
{
	return at<Section>(name);
}

Group &Group::group(const std::string &name)
{
	return at<Group>(name);
}

const Group &Group::group(const std::string &name) const
{
	return at<Group>(name);
}

}

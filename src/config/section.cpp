#include <section.h>

#include <QDebug>

namespace Config
{

Section::Section(tinyxml2::XMLElement *root, YAML::Node &section)
	:NodeList(root->Attribute("name"))
{
	// Creating children depending on their type
	tinyxml2::XMLElement *child = root->FirstChildElement();
	while (child) {
		addVariable(child, section);
		child = child->NextSiblingElement();
	}

	updateNameToIndexMap();
}

Variable *Section::createVariable(tinyxml2::XMLElement *elem, YAML::Node &section)
{
	const std::string name = elem->Attribute("name");
	const std::string typeName = elem->Attribute("type");

	if (typeName == "float") {
		return new Variable(name, Variable::Type::FLOAT, section, elem->FloatText());
	}
	else if (typeName == "int") {
		return new Variable(name, Variable::Type::INT, section, elem->IntText());
	}
	else if (typeName == "string") {
		return new Variable(name, Variable::Type::STRING, section, elem->GetText());
	}

	return nullptr;
}

void Section::addVariable(tinyxml2::XMLElement *elem, YAML::Node &section)
{
	Variable *var = createVariable(elem, section);
	m_children.emplace_back(var);
}

}

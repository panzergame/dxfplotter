#include <unordered_map>
#include <functional>

#include <section.h>

namespace Config
{

Section::Section(tinyxml2::XMLElement *root, YAML::Node &section)
	:NodeList(root->Attribute("name"))
{
	// Creating children depending on their type
	tinyxml2::XMLElement *child = root->FirstChildElement();
	while (child) {
		addVariable(child, section);
	}

	updateNameToIndexMap();
}

void Section::addVariable(tinyxml2::XMLElement *elem, YAML::Node &section)
{
	const std::string name = elem->Attribute("name");

	static const std::unordered_map<std::string, std::function<Variable()>> creator = {
		{"float", [&name, &section, &elem](){
			return Variable(name, Variable::Type::FLOAT, section, elem->FloatText());
		}},
		{"int", [&name, &section, &elem](){
			return Variable(name, Variable::Type::FLOAT, section, elem->FloatText());
		}},
		{"string", [&name, &section, &elem](){
			return Variable(name, Variable::Type::FLOAT, section, elem->FloatText());
		}},
	};

	const char *typeName = elem->Attribute("type");
	m_children.push_back(creator.at(typeName)());
}

}

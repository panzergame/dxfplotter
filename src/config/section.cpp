#include <unordered_map>

#include <section.h>

namespace Config
{

Section::Section(tinyxml2::XMLElement *root)
	:NodeList(root->Attribute("name"))
{
	// Creating children depending on their type
	tinyxml2::XMLElement *child = root->FirstChildElement();
	while (child) {
		addVariable(child);
	}

	updateNameToIndexMap();
}

void Section::addVariable(tinyxml2::XMLElement *elem)
{
	static const std::unordered_map<std::string, Variable::Type> nameToType = {
		{"float", Variable::Type::FLOAT},
		{"int", Variable::Type::INT},
		{"string", Variable::Type::STRING}
	};

	const char *typeName = elem->Attribute("type");
	const std::string name = elem->Attribute("name");

	Variable::Type type = nameToType.at(typeName);

	Variable &variable = m_children.emplace_back(name, type);

	switch (type) {
		case Variable::Type::FLOAT:
		{
			variable.init(elem->FloatText());
			break;
		}
		case Variable::Type::INT:
		{
			variable.init(elem->IntText());
			break;
		}
		case Variable::Type::STRING:
		{
			variable.init(std::string(elem->GetText()));
			break;
		}
	}
}

}

#include <config.h>

#include <tinyxml2.h>

#include <QDebug> // TODO

namespace Config
{

extern const char *CONFIG_RAW_XML_STRING;

Config::Config(const std::string &filePath)
	:m_filePath(filePath)
{
	YAML::Node root = YAML::LoadFile(filePath);

	// Load XML content
	tinyxml2::XMLDocument doc;
	const tinyxml2::XMLError parseStatus = doc.Parse(CONFIG_RAW_XML_STRING);

	assert(parseStatus == tinyxml2::XML_SUCCESS);

	// Instantiation of root group
	m_root = Group(doc.FirstChildElement(), root);
}

Config::~Config()
{
	save();
}

Group &Config::root()
{
	return m_root;
}

const Group &Config::root() const
{
	return m_root;
}

void Config::save()
{
	// TODO
}

}

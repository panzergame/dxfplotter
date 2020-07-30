#include <config.h>

#include <tinyxml2.h>

#include <QDebug> // TODO

namespace Config
{

extern const char *CONFIG_RAW_XML_STRING;

Config::Config(const std::string &filePath, tinyxml2::XMLElement *root)
	:Group(root),
	m_filePath(filePath),
	m_file(m_filePath)
{
}

Config Config::Create(const std::string &filePath)
{
	// Load XML content
	tinyxml2::XMLDocument doc;
	const tinyxml2::XMLError parseStatus = doc.Parse(CONFIG_RAW_XML_STRING);

	assert(parseStatus == tinyxml2::XML_SUCCESS);

	return Config(filePath, doc.FirstChildElement());
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
	m_file.Save(m_filePath);
}

}

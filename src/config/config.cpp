#include <config.h>

#include <QDebug>

#include <fstream>

namespace Config
{

extern const char *CONFIG_RAW_XML_STRING;

Config::Config(const std::string &filePath)
	:m_filePath(filePath)
{
	try {
		m_yamlRoot = YAML::LoadFile(filePath);
	}
	catch (const YAML::BadFile &e) {
		qInfo() << "Initializing configuration from defaults";
	}

	// Instantiation of root group
	m_root = Root(m_yamlRoot);
}

Config::~Config()
{
	save();
}

Root &Config::root()
{
	return m_root;
}

const Root &Config::root() const
{
	return m_root;
}

void Config::save()
{
	std::ofstream outStream(m_filePath);
	outStream << m_yamlRoot;
}

}

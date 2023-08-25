#include <config.h>

#include <QDebug>

#include <fstream>

namespace config
{

Config::Config(const std::string &filePath)
	:m_filePath(filePath)
{
	try {
		m_yamlRoot = YAML::LoadFile(filePath);
	}
	catch (const YAML::BadFile&) {
		qInfo() << "Initializing configuration from defaults";
	}

	// Instantiation of root group
	m_root = Root(m_yamlRoot);
}

Config::Config(const Config &other)
	:m_filePath(other.m_filePath),
	m_yamlRoot(Clone(other.m_yamlRoot)),
	m_root(m_yamlRoot)
{
}

Config::~Config()
{
}

Root &Config::root()
{
	return m_root;
}

const Root &Config::root() const
{
	return m_root;
}

void Config::save() const
{
	std::ofstream outStream(m_filePath);
	outStream << m_yamlRoot;
}

}

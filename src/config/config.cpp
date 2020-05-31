#include <config.h>

namespace Config
{

Config::~Config()
{
	save();
}

void Config::save()
{
	m_file.Save(m_filePath);
}

}

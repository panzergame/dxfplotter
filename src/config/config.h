#pragma once

#include <config/group.h>

namespace Config
{

class Config
{
private:
	std::string m_filePath;
	YAML::Node m_yamlRoot;
	Group m_root;

public:
	explicit Config(const std::string &filePath);
	~Config();

	Group& root();
	const Group& root() const;

	void save();
};

}


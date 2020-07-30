#pragma once

#include <config/group.h>

namespace Config
{

class Config : public Group
{
private:
	std::string m_filePath;
	INI::File m_file;
	Group m_root;

	explicit Config(const std::string &filePath, tinyxml2::XMLElement *root);

public:
	static Config Create(const std::string &filePath);
	~Config();

	Group& root();
	const Group& root() const;

	void save();
};

}


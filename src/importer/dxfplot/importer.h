#pragma once

#include <config/config.h>

#include <model/document.h>

namespace Importer::Dxfplot
{

class Importer
{
private:
	const Config::Tools &m_tools;
	const Config::Profiles &m_profiles;

public:
	explicit Importer(const Config::Tools &tools, const Config::Profiles &profiles);

	Model::Document::UPtr operator()(const std::string &fileName) const;
};

}

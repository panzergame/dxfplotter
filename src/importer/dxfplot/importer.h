#pragma once

#include <config/config.h>

#include <model/document.h>

#include <cereal/archives/json.hpp> // TODO

namespace Importer::Dxfplot
{

class Importer
{
private:
	using Archive = cereal::JSONInputArchive;

	const Config::Tools &m_tools;
	const Config::Profiles &m_profiles;

public:
	explicit Importer(const Config::Tools &tools, const Config::Profiles &profiles);

	Model::Document::UPtr operator()(const std::string &fileName) const;
	Model::Document::UPtr operator()(std::istream& input) const;
};

}

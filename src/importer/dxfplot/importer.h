#pragma once

#include <config/config.h>

#include <model/document.h>

#include <cereal/archives/json.hpp> // TODO

namespace importer::dxfplot
{

class Importer
{
private:
	using Archive = cereal::JSONInputArchive;

	const config::Tools &m_tools;
	const config::Profiles &m_profiles;

public:
	explicit Importer(const config::Tools &tools, const config::Profiles &profiles);

	model::Document::UPtr operator()(const std::string &fileName) const;
	model::Document::UPtr operator()(std::istream& input) const;
};

}

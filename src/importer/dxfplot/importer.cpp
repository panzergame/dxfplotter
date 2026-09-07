module;

#include <serializer/access.h>

#include <fstream>
#include <cereal/archives/json.hpp>

export module importer.dxfplot.importer;

import common.exception;
import config.config;
import model.document;
import serializer.task;
import model.task;

export namespace importer::dxfplot
{

class Importer
{
private:
	using Archive = cereal::JSONInputArchive;

	const config::Tools& m_tools;
	const config::Profiles& m_profiles;

public:
	explicit Importer(const config::Tools& tools, const config::Profiles& profiles);

	model::Document::UPtr operator()(const std::string& fileName) const;
	model::Document::UPtr operator()(std::istream& input) const;
};

}

namespace importer::dxfplot
{

Importer::Importer(const config::Tools& tools, const config::Profiles& profiles)
	: m_tools(tools)
	, m_profiles(profiles)
{
}

model::Document::UPtr Importer::operator()(const std::string& fileName) const
{
	std::ifstream input(fileName);

	return (*this)(input);
}

model::Document::UPtr Importer::operator()(std::istream& input) const
{
	Archive archive(input);

	model::Task::UPtr task = std::make_unique<model::Task>();
	archive(cereal::make_nvp("task", *task));

	std::string profileName;
	archive(cereal::make_nvp("profile_name", profileName));

	std::string toolName;
	archive(cereal::make_nvp("tool_name", toolName));

	const config::Tools::Tool* tool = m_tools.get(toolName);
	const config::Profiles::Profile* profile = m_profiles.get(profileName);

	if (!tool) {
		throw common::ImportCouldNotFindToolConfigException();
	}
	if (!profile) {
		throw common::ImportCouldNotFindProfileConfigException();
	}

	return std::make_unique<model::Document>(std::move(task), *tool, *profile);
}

}

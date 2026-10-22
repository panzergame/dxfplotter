module;

#include <fstream>
#include <cereal/archives/json.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>

export module importer.dxfplot.importer;

import common.exception;
import config.config;
import model.document;
import serializer.access;
import serializer.bulge;
import serializer.layer;
import serializer.offsettedpath;
import serializer.path;
import serializer.pathsettings;
import serializer.polyline;
import serializer.qvector2d;
import serializer.renderable;
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
	explicit Importer(const config::Tools& tools, const config::Profiles& profiles)
		: m_tools(tools)
		, m_profiles(profiles)
	{
	}

	model::Document::UPtr operator()(const std::string& fileName) const
	{
		std::ifstream input(fileName);

		return (*this)(input);
	}

	model::Document::UPtr operator()(std::istream& input) const
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
};

}

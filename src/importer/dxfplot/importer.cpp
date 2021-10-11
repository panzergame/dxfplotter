#include <importer.h>

#include <fstream>

#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>

#include <serializer/task.h>

#include <common/exception.h>

namespace Importer::Dxfplot
{

Importer::Importer(const Config::Tools &tools, const Config::Profiles &profiles)
	:m_tools(tools),
	m_profiles(profiles)
{
}

Model::Document::UPtr Importer::operator()(const std::string &fileName) const
{
	std::ifstream input(fileName);

	return (*this)(input);
}

Model::Document::UPtr Importer::operator()(std::istream& input) const
{
	Archive archive(input);

	Model::Task::UPtr task = std::make_unique<Model::Task>();
	archive(cereal::make_nvp("task", *task));

	std::string profileName;
	archive(cereal::make_nvp("profile_name", profileName));

	std::string toolName;
	archive(cereal::make_nvp("tool_name", toolName));

	const Config::Tools::Tool *tool = m_tools.get(toolName);
	const Config::Profiles::Profile *profile = m_profiles.get(profileName);

	if (!tool) {
		throw Common::ImportCouldNotFindToolConfigException();
	}
	if (!profile) {
		throw Common::ImportCouldNotFindProfileConfigException();
	}

	return std::make_unique<Model::Document>(std::move(task), *tool, *profile);
}

}

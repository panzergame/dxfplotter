#include <importer.h>
#include <document.h>

#include <fstream>

#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>

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
	cereal::JSONInputArchive archive(input);
	Model::Document::UPtr document;
 	archive(CEREAL_NVP(document));

	return nullptr;
}

}

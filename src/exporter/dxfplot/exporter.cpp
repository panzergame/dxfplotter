#include <exporter.h>

#include <task.h>

#include <cereal/cereal.hpp>

namespace Exporter::Dxfplot
{

void Exporter::operator()(const Model::Document& document, std::ostream &output)  const
{
	Archive archive(output);
	save(archive, document);
}

void Exporter::save(Archive &archive, const Model::Document& document) const
{
	archive(cereal::make_nvp("task", document.task()));
	archive(cereal::make_nvp("profile_name", document.profileConfig().name()));
	archive(cereal::make_nvp("tool_name", document.toolConfig().name()));
}

}

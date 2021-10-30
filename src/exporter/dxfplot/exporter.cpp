#include <exporter.h>

#include <serializer/task.h>

#include <cereal/cereal.hpp>

namespace exporter::dxfplot
{

void Exporter::operator()(const model::Document& document, std::ostream &output)  const
{
	Archive archive(output);
	save(archive, document);
}

void Exporter::save(Archive &archive, const model::Document& document) const
{
	archive(cereal::make_nvp("task", document.task()));
	archive(cereal::make_nvp("profile_name", document.profileConfig().name()));
	archive(cereal::make_nvp("tool_name", document.toolConfig().name()));
}

}

module;

#include <serializer/access.h>

#include <fstream>
#include <cereal/archives/json.hpp>

export module exporter.dxfplot.exporter;

import model.document;
import serializer.task;

export namespace exporter::dxfplot
{

class Exporter
{
public:
	explicit Exporter() = default;

	void operator()(const model::Document& document, std::ostream& output) const;

private:
	using Archive = cereal::JSONOutputArchive;

	void save(Archive& archive, const model::Document& document) const;
};

}

namespace exporter::dxfplot
{

void Exporter::operator()(const model::Document& document, std::ostream& output) const
{
	Archive archive(output);
	save(archive, document);
}

void Exporter::save(Archive& archive, const model::Document& document) const
{
	archive(cereal::make_nvp("task", document.task()));
	archive(cereal::make_nvp("profile_name", document.profileConfig().name()));
	archive(cereal::make_nvp("tool_name", document.toolConfig().name()));
}

}

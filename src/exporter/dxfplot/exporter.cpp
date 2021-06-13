#include <exporter.h>

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>

#include <document.h>

namespace Exporter::Dxfplot
{

void Exporter::operator()(const Model::Document& document, std::ostream &output)  const
{
	cereal::JSONOutputArchive archive(output);
	archive(CEREAL_NVP(document));
}

}

#include <gtest/gtest.h>

#include <exporter/dxfplot/exporter.h>
#include <importer/dxfplot/importer.h>
#include <config/config.h>
#include <model/document.h>

#include <sstream>

TEST(DxfplotImporterTest, shouldReimportDocumentWithToolAndProfileConfig)
{
	std::ostringstream output;
	const Config::Tools::Tool tool("tool", YAML::Node());
	const Config::Profiles::Profile profile("profile", YAML::Node());
	const Config::Profiles::Profile::Gcode gcode("gcode", YAML::Node());

	Model::Layer *layer = new Model::Layer("");
	const Geometry::Bulge bulge(QVector2D(0, 0), QVector2D(1, 1), 0);
	Geometry::Polyline polyline({bulge});
	Model::PathSettings settings(10, 10, 10, 10);
	Model::Path *path = new Model::Path(std::move(polyline), "", settings, *layer);

	Model::Path::ListUPtr paths;
	paths.emplace_back(path);
	layer->setChildren(std::move(paths));

	Model::Layer::ListUPtr layers;
	layers.emplace_back(layer);
	Model::Task::UPtr task = std::make_unique<Model::Task>(std::move(layers));
	Model::Document document(std::move(task), tool, profile);

	{
		Exporter::Dxfplot::Exporter exporter;
		exporter(document, output);
	}


	YAML::Node toolsNode;
	toolsNode["tool"] = YAML::Node();
	
	YAML::Node profilesNode;
	profilesNode["profile"] = YAML::Node();
	
	Config::Tools tools{toolsNode};
	Config::Profiles profiles{profilesNode};
	
	Importer::Dxfplot::Importer importer(tools, profiles);

	{
		std::istringstream input;
		input.str(output.str());
		Model::Document::UPtr document = importer(input);
	}
	
	std::cout << output.str() << std::endl;
}


#include <exporterfixture.h>

#include <exporter/dxfplot/exporter.h>

#include <sstream>

TEST_F(ExporterFixture, shouldRender)
{
	std::ostringstream output;
	const Config::Tools::Tool tool("tool", YAML::Node());
	const Config::Profiles::Profile profile("profile", YAML::Node());
	const Config::Profiles::Profile::Gcode gcode("gcode", YAML::Node());

	const Geometry::Bulge bulge(QVector2D(0, 0), QVector2D(1, 1), 0);
	Geometry::Polyline polyline({bulge});
	Model::PathSettings settings(10, 10, 10, 10);
	Model::Path *path = new Model::Path(std::move(polyline), "", settings);

	Model::Path::ListUPtr paths;
	paths.emplace_back(path);
	Model::Layer *layer = new Model::Layer("", std::move(paths));

	Model::Layer::ListUPtr layers;
	layers.emplace_back(layer);

	Model::Task::UPtr task = std::make_unique<Model::Task>(std::move(layers));
	Model::Document document(std::move(task), tool, profile);

	{
		Exporter::Dxfplot::Exporter exporter;
		exporter(document, output);
	}

	std::cout << output.str() << std::endl;
}


#include <gtest/gtest.h>

#include <exporter/gcode/exporter.h>
#include <config/config.h>
#include <model/task.h>

#include <sstream>

TEST(ExporterTest, shouldRenderAllPathsWhenAllVisible)
{
	std::ostringstream output;
	const Config::Tools::Tool tool("tool", YAML::Node());
	const Config::Profiles::Profile::Gcode gcode("gcode", YAML::Node());

	Model::Layer *layer = new Model::Layer("");
	const Geometry::Bulge bulge(QVector2D(0, 0), QVector2D(1, 1), 0);
	Geometry::Polyline polyline({bulge});
	Model::PathSettings settings(10, 10, 10, 10);
	Model::Path *path = new Model::Path(std::move(polyline), "", settings, *layer);

	Model::Path::ListUPtr paths;
	paths.emplace_back(path);

	Model::Layer::ListUPtr layers;
	layers.emplace_back(layer);
	Model::Task task(std::move(paths), std::move(layers));

	const Exporter::GCode::Exporter exporter(task, tool, gcode, output);

	EXPECT_EQ(R"(G0 Z 1.000
G0 X 0.000 Y 0.000
M4 S 10.000
G1 Z -1.000 F 10.000
G1 X 1.000 Y 1.000 F 10.000
G1 Z -2.000 F 10.000
G1 X 0.000 Y 0.000 F 10.000
G1 Z -3.000 F 10.000
G1 X 1.000 Y 1.000 F 10.000
G1 Z -4.000 F 10.000
G1 X 0.000 Y 0.000 F 10.000
G1 Z -5.000 F 10.000
G1 X 1.000 Y 1.000 F 10.000
G1 Z -6.000 F 10.000
G1 X 0.000 Y 0.000 F 10.000
G1 Z -7.000 F 10.000
G1 X 1.000 Y 1.000 F 10.000
G1 Z -8.000 F 10.000
G1 X 0.000 Y 0.000 F 10.000
G1 Z -9.000 F 10.000
G1 X 1.000 Y 1.000 F 10.000
G1 Z -10.000 F 10.000
G1 X 0.000 Y 0.000 F 10.000
G0 Z 1.000
M5
G0 X 0.000 Y 0.000
)", output.str());
}


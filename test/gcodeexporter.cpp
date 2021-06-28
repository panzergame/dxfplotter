#include <gtest/gtest.h>

#include <exporter/gcode/exporter.h>
#include <config/config.h>
#include <model/document.h>

#include <sstream>

class ExporterTest : public ::testing::Test
{
protected:
	const Config::Tools::Tool m_tool{"tool", YAML::Node()};
	const Config::Profiles::Profile::Gcode m_gcode{"gcode", YAML::Node()};
	const Config::Profiles::Profile m_profile{"profile", YAML::Node()};
	const Model::PathSettings m_settings{10, 10, 10, 0.1};
	Model::Task::UPtr m_task;
	Model::Document::UPtr m_document;
	std::ostringstream m_output;

	void createTaskFromPolyline(Geometry::Polyline &&polyline)
	{
		Model::Layer::UPtr layer = std::make_unique<Model::Layer>("layer");
		Model::Path::UPtr path = std::make_unique<Model::Path>(std::move(polyline), "", m_settings, *layer);

		Model::Path::ListUPtr paths;
		paths.push_back(std::move(path));

		Model::Layer::ListUPtr layers;
		layers.push_back(std::move(layer));
		m_task = std::make_unique<Model::Task>(std::move(layers));
		m_document = std::make_unique<Model::Document>(std::move(m_task), m_tool, m_profile);
	}
};

TEST_F(ExporterTest, shouldRenderAllPathsWhenAllVisible)
{
	const Geometry::Bulge bulge(QVector2D(0, 0), QVector2D(1, 1), 0);
	Geometry::Polyline polyline({bulge});

	Model::Path::ListUPtr paths;
	paths.emplace_back(path);
	layer->setChildren(std::move(paths));

	Model::Layer::ListUPtr layers;
	layers.emplace_back(layer);
	Model::Task::UPtr task = std::make_unique<Model::Task>(std::move(layers));
	Model::Document::UPtr = document(std::move(task), tool, profile);

	{
		Exporter::GCode::Exporter exporter(tool, gcode);
		exporter(document, output);
	}

	EXPECT_EQ(R"(G0 Z 1.000
G0 X 0.000 Y 0.000
M4 S 10.000
G1 Z -0.000 F 10.000
G1 X 1.000 Y 1.000 F 10.000
G1 Z -0.100 F 10.000
G1 X 0.000 Y 0.000 F 10.000
G0 Z 1.000
M5
G0 X 0.000 Y 0.000
)", m_output.str());
}

TEST_F(ExporterTest, shouldRenderOffsetedRightCwTriangleBuCutBackward)
{
	const Geometry::Bulge b1(QVector2D(0, 0), QVector2D(1, 1), 0);
	const Geometry::Bulge b2(QVector2D(1, 1), QVector2D(1, 0), 0);
	const Geometry::Bulge b3(QVector2D(1, 0), QVector2D(0, 0), 0);
	Geometry::Polyline polyline({b1, b2, b3});

	ASSERT_TRUE(polyline.isClosed());

	createTaskFromPolyline(std::move(polyline));

	m_task->forEachPath([](Model::Path& path){
		path.offset(-0.2f, 0.0f, 0.0f);
	});

	const Exporter::GCode::Exporter exporter(m_tool, m_gcode);
	exporter(*m_document, m_output);

	EXPECT_EQ(R"(G0 Z 1.000
G0 X 0.483 Y 0.200
M4 S 10.000
G1 Z -0.000 F 10.000
G1 X 0.800 Y 0.200 F 10.000
G1 X 0.800 Y 0.517 F 10.000
G1 X 0.483 Y 0.200 F 10.000
G1 Z -0.100 F 10.000
G1 X 0.800 Y 0.200 F 10.000
G1 X 0.800 Y 0.517 F 10.000
G1 X 0.483 Y 0.200 F 10.000
G0 Z 1.000
M5
G0 X 0.000 Y 0.000
)", m_output.str());
}

TEST_F(ExporterTest, shouldRenderOffsetedLeftCwTriangleBuCutForward)
{
	const Geometry::Bulge b1(QVector2D(0, 0), QVector2D(1, 1), 0);
	const Geometry::Bulge b2(QVector2D(1, 1), QVector2D(1, 0), 0);
	const Geometry::Bulge b3(QVector2D(1, 0), QVector2D(0, 0), 0);
	Geometry::Polyline polyline({b1, b2, b3});

	ASSERT_TRUE(polyline.isClosed());

	createTaskFromPolyline(std::move(polyline));

	m_task->forEachPath([](Model::Path& path){
		path.offset(0.2f, 0.0f, 0.0f);
	});

	const Exporter::GCode::Exporter exporter(m_tool, m_gcode);
	exporter(*m_document, m_output);

	EXPECT_EQ(R"(G0 Z 1.000
G0 X -0.141 Y 0.141
M4 S 10.000
G1 Z -0.000 F 10.000
G1 X 0.859 Y 1.141 F 10.000
G2 X 1.200 Y 1.000 I 0.141 J -0.141 F 10.000
G1 X 1.200 Y 0.000 F 10.000
G2 X 1.000 Y -0.200 I -0.200 J 0.000 F 10.000
G1 X 0.000 Y -0.200 F 10.000
G2 X -0.141 Y 0.141 I 0.000 J 0.200 F 10.000
G1 Z -0.100 F 10.000
G1 X 0.859 Y 1.141 F 10.000
G2 X 1.200 Y 1.000 I 0.141 J -0.141 F 10.000
G1 X 1.200 Y 0.000 F 10.000
G2 X 1.000 Y -0.200 I -0.200 J 0.000 F 10.000
G1 X 0.000 Y -0.200 F 10.000
G2 X -0.141 Y 0.141 I 0.000 J 0.200 F 10.000
G0 Z 1.000
M5
G0 X 0.000 Y 0.000
)", m_output.str());
}

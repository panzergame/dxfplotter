#include <exporterfixture.h>

#include <exporter/gcode/exporter.h>

#include <sstream>

TEST_F(ExporterFixture, shouldRenderAllPathsWhenAllVisible)
{
	const geometry::Bulge bulge(QVector2D(0, 0), QVector2D(1, 1), 0);
	geometry::Polyline polyline({bulge});

	createTaskFromPolyline(std::move(polyline));

	const exporter::gcode::Exporter exporter(m_tool, m_profile);
	exporter(*m_document, m_output);

	EXPECT_EQ(R"(G0 Z 1.000
G0 X 0.000 Y 0.000
M4 S 10.000
G1 Z -0.100 F 10.000
G1 X 1.000 Y 1.000 F 10.000
G0 Z 1.000
M5
G0 X 0.000 Y 0.000
)", m_output.str());
}

TEST_F(ExporterFixture, shouldRenderOffsetedRightCwTriangleBeCutBackward)
{
	const geometry::Bulge b1(QVector2D(0, 0), QVector2D(1, 1), 0);
	const geometry::Bulge b2(QVector2D(1, 1), QVector2D(1, 0), 0);
	const geometry::Bulge b3(QVector2D(1, 0), QVector2D(0, 0), 0);
	geometry::Polyline polyline({b1, b2, b3});

	ASSERT_TRUE(polyline.isClosed());

	createTaskFromPolyline(std::move(polyline));

	m_task->forEachPath([](model::Path& path){
		path.offset(-0.2f, 0.0f, 0.0f);
	});

	const exporter::gcode::Exporter exporter(m_tool, m_profile);
	exporter(*m_document, m_output);

	EXPECT_EQ(R"(G0 Z 1.000
G0 X 0.483 Y 0.200
M4 S 10.000
G1 Z -0.100 F 10.000
G1 X 0.800 Y 0.200 F 10.000
G1 X 0.800 Y 0.517 F 10.000
G1 X 0.483 Y 0.200 F 10.000
G0 Z 1.000
M5
G0 X 0.000 Y 0.000
)", m_output.str());
}

TEST_F(ExporterFixture, shouldRenderOffsetedLeftCwTriangleBeCutForward)
{
	const geometry::Bulge b1(QVector2D(0, 0), QVector2D(1, 1), 0);
	const geometry::Bulge b2(QVector2D(1, 1), QVector2D(1, 0), 0);
	const geometry::Bulge b3(QVector2D(1, 0), QVector2D(0, 0), 0);
	geometry::Polyline polyline({b1, b2, b3});

	ASSERT_TRUE(polyline.isClosed());

	createTaskFromPolyline(std::move(polyline));

	m_task->forEachPath([](model::Path& path){
		path.offset(0.2f, 0.0f, 0.0f);
	});

	const exporter::gcode::Exporter exporter(m_tool, m_profile);
	exporter(*m_document, m_output);

	EXPECT_EQ(R"(G0 Z 1.000
G0 X -0.141 Y 0.141
M4 S 10.000
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

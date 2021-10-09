#include <exporterfixture.h>

#include <exporter/gcode/exporter.h>

#include <sstream>

TEST_F(ExporterFixture, shouldRenderAllPathsWhenAllVisible)
{
	const Geometry::Bulge bulge(QVector2D(0, 0), QVector2D(1, 1), 0);
	Geometry::Polyline polyline({bulge});

	createTaskFromPolyline(std::move(polyline));

	const Exporter::GCode::Exporter exporter(m_tool, m_gcode);
	exporter(*m_document, m_output);

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

TEST_F(ExporterFixture, shouldRenderOffsetedRightCwTriangleBeCutBackward)
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

TEST_F(ExporterFixture, shouldRenderOffsetedLeftCwTriangleBeCutForward)
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

#include <exporterfixture.h>

#include <exporter/dxfplot/exporter.h>

#include <sstream>

TEST_F(ExporterFixture, shouldExportNotEmpty)
{

	const Geometry::Bulge bulge(QVector2D(0, 0), QVector2D(1, 1), 0);
	Geometry::Polyline polyline({bulge});

	createTaskFromPolyline(std::move(polyline));

	std::ostringstream output;
	Exporter::Dxfplot::Exporter exporter;
	exporter(*m_document, output);

	const std::string outputContent = output.str();
	EXPECT_GT(outputContent.size(), 0);
}


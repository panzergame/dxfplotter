#include <exporterfixture.h>

#include <exporter/dxfplot/exporter.h>

#include <sstream>

TEST_F(ExporterFixture, shouldExportNotEmpty)
{

	const geometry::Bulge bulge(Eigen::Vector2d::Zero(), Eigen::Vector2d::Ones(), 0);
	geometry::Polyline polyline({bulge});

	createTaskFromPolyline(std::move(polyline));

	std::ostringstream output;
	exporter::dxfplot::Exporter exporter;
	exporter(*m_document, output);

	const std::string outputContent = output.str();
	EXPECT_GT(outputContent.size(), 0);
}


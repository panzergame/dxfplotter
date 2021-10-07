#include <exporterfixture.h>

#include <exporter/dxfplot/exporter.h>
#include <importer/dxfplot/importer.h>

#include <sstream>

TEST_F(ExporterFixture, shouldReimportDocumentWithToolAndProfileConfig)
{
	std::ostringstream output;

	Geometry::Polyline polyline;
	createTaskFromPolyline(std::move(polyline));

	{
		Exporter::Dxfplot::Exporter exporter;
		exporter(*m_document, output);
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

		EXPECT_EQ(document->toolConfig().name(), "tool");
		EXPECT_EQ(document->profileConfig().name(), "profile");
	}
}

TEST_F(ExporterFixture, shouldReimportDocumentWithSameTask)
{
	std::ostringstream output;

	const Geometry::Bulge bulge(QVector2D(0, 0), QVector2D(1, 1), 0);
	Geometry::Polyline polyline({bulge});

	createTaskFromPolyline(std::move(polyline));

	{
		Exporter::Dxfplot::Exporter exporter;
		exporter(*m_document, output);
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

		std::cout << output.str() << std::endl;

		Model::Task &task = document->task();
		EXPECT_EQ(task.pathCount(), 1);

		const Geometry::Polyline &firstPathPolyline = task.pathAt(0).basePolyline();
		EXPECT_EQ(polyline, firstPathPolyline);
	}

}


#include <exporterfixture.h>

#include <exporter/dxfplot/exporter.h>
#include <importer/dxfplot/importer.h>

#include <common/exception.h>

#include <sstream>


TEST_F(ExporterFixture, shouldThrowExceptionWhenToolNotFound)
{
	std::ostringstream output;

	const geometry::Bulge bulge(QVector2D(0, 0), QVector2D(1, 1), 0);
	geometry::Polyline polyline({bulge});

	createTaskFromPolyline(std::move(polyline));

	exporter::dxfplot::Exporter exporter;
	exporter(*m_document, output);

	YAML::Node toolsNode;	
	YAML::Node profilesNode;
	profilesNode["profile"] = YAML::Node();
	
	config::Tools tools{toolsNode};
	config::Profiles profiles{profilesNode};
	
	importer::dxfplot::Importer importer(tools, profiles);

	std::istringstream input;
	input.str(output.str());
	
	EXPECT_THROW(importer(input), common::ImportCouldNotFindToolConfigException);
}

TEST_F(ExporterFixture, shouldThrowExceptionWhenProfileNotFound)
{
	std::ostringstream output;

	const geometry::Bulge bulge(QVector2D(0, 0), QVector2D(1, 1), 0);
	geometry::Polyline polyline({bulge});

	createTaskFromPolyline(std::move(polyline));

	exporter::dxfplot::Exporter exporter;
	exporter(*m_document, output);

	YAML::Node toolsNode;
	toolsNode["tool"] = YAML::Node();
	YAML::Node profilesNode;
	
	config::Tools tools{toolsNode};
	config::Profiles profiles{profilesNode};
	
	importer::dxfplot::Importer importer(tools, profiles);

	std::istringstream input;
	input.str(output.str());
	
	EXPECT_THROW(importer(input), common::ImportCouldNotFindProfileConfigException);
}

TEST_F(ExporterFixture, shouldReimportDocumentWithToolAndProfileConfig)
{
	std::ostringstream output;

	const geometry::Bulge bulge(QVector2D(0, 0), QVector2D(1, 1), 0);
	geometry::Polyline polyline({bulge});

	createTaskFromPolyline(std::move(polyline));

	exporter::dxfplot::Exporter exporter;
	exporter(*m_document, output);

	YAML::Node toolsNode;
	toolsNode["tool"] = YAML::Node();
	
	YAML::Node profilesNode;
	profilesNode["profile"] = YAML::Node();
	
	config::Tools tools{toolsNode};
	config::Profiles profiles{profilesNode};
	
	importer::dxfplot::Importer importer(tools, profiles);

	std::istringstream input;
	input.str(output.str());
	model::Document::UPtr document = importer(input);

	EXPECT_EQ(document->toolConfig().name(), "tool");
	EXPECT_EQ(document->profileConfig().name(), "profile");
}

TEST_F(ExporterFixture, shouldReimportDocumentWithSameTask)
{
	std::ostringstream output;

	const geometry::Bulge bulge(QVector2D(0, 0), QVector2D(1, 1), 0);
	geometry::Polyline polyline({bulge});

	createTaskFromPolyline(std::move(polyline));

	exporter::dxfplot::Exporter exporter;
	exporter(*m_document, output);

	YAML::Node toolsNode;
	toolsNode["tool"] = YAML::Node();
	
	YAML::Node profilesNode;
	profilesNode["profile"] = YAML::Node();
	
	config::Tools tools{toolsNode};
	config::Profiles profiles{profilesNode};
	
	importer::dxfplot::Importer importer(tools, profiles);

	std::istringstream input;
	input.str(output.str());
	model::Document::UPtr document = importer(input);

	std::cout << output.str() << std::endl;

	model::Task &task = document->task();
	EXPECT_EQ(task.pathCount(), 1);

	const geometry::Polyline &firstPathPolyline = task.pathAt(0).basePolyline();
	EXPECT_EQ(polyline, firstPathPolyline);
}


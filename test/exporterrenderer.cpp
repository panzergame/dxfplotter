#include <exporterfixture.h>

#include <exporter/renderer/renderer.h>

class DepthTrackerVisitor
{
public:
	std::vector<float> depths;

	void start(const QVector2D& from, float safetyDepth)
	{
	}

	void end(const QVector2D& to, float safetyDepth)
	{
	}

	void startOperation(const QVector2D& to, float intensity)
	{
	}

	void endOperation(float safetyDepth)
	{
	}

	void processPathAtDepth(const geometry::Polyline& polyline, float depth, float planeFeedRate, float depthFeedRate)
	{
		depths.push_back(depth) ;
	}
};

class ExporterRendererFixture : public ExporterFixture
{
};

TEST_F(ExporterRendererFixture, shouldCutFirstPassAtZeroDepth)
{
	const geometry::Bulge bulge(QVector2D(0, 0), QVector2D(1, 1), 0);
	geometry::Polyline polyline({bulge});

	createTaskFromPolyline(std::move(polyline));

	config::Profiles::Profile profile = m_profile;
	profile.cut().passAtZeroDepth() = true;

	config::Tools::Tool tool = m_tool;
	tool.general().depthPerCut() = 0.02f;

	DepthTrackerVisitor visitor;
	exporter::renderer::Renderer renderer(tool, profile, visitor);
	renderer.render(*m_document);

	const int nbCut = 0.1f / 0.02f + 2;
	EXPECT_EQ(visitor.depths.size(), nbCut);

	EXPECT_FLOAT_EQ(visitor.depths.front(), 0.0f);
	EXPECT_FLOAT_EQ(visitor.depths.back(), -0.1f);
}

TEST_F(ExporterRendererFixture, shouldCutFirstPassAtToolDepth)
{
	const geometry::Bulge bulge(QVector2D(0, 0), QVector2D(1, 1), 0);
	geometry::Polyline polyline({bulge});

	createTaskFromPolyline(std::move(polyline));

	config::Profiles::Profile profile = m_profile;
	profile.cut().passAtZeroDepth() = false;

	config::Tools::Tool tool = m_tool;
	tool.general().depthPerCut() = 0.02f;

	DepthTrackerVisitor visitor;
	exporter::renderer::Renderer renderer(tool, profile, visitor);
	renderer.render(*m_document);

	const int nbCut = 0.1f / 0.02f + 1;
	EXPECT_EQ(visitor.depths.size(), nbCut);

	EXPECT_FLOAT_EQ(visitor.depths.front(), -0.02f);
	EXPECT_FLOAT_EQ(visitor.depths.back(), -0.1f);
}

TEST_F(ExporterRendererFixture, shouldCutPathSingleDepth)
{
	const geometry::Bulge bulge(QVector2D(0, 0), QVector2D(1, 1), 0);
	geometry::Polyline polyline({bulge});

	createTaskFromPolyline(std::move(polyline));

	config::Tools::Tool tool = m_tool;
	tool.general().depthPerCut() = 0.2f;

	DepthTrackerVisitor visitor;
	exporter::renderer::Renderer renderer(tool, m_profile, visitor);
	renderer.render(*m_document);

	const int nbCut = 1;
	EXPECT_EQ(visitor.depths.size(), nbCut);

	EXPECT_FLOAT_EQ(visitor.depths.front(), -0.1f);
}



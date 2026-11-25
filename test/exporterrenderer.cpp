
#include <QVector2D>
#include <QtMath>
#include <exporterfixture.h>

import exporter.renderer.renderer;
import geometry.bulge;
import geometry.polyline;
import geometry.tooloperation.dragknife;
import geometry.utils;
import model.path;

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

class EventTrackerVisitor
{
public:
	struct Event
	{
		enum class Type { StartOperation, Path, EndOperation };

		Type type;
		QVector2D point;
		float depth;
	};

	std::vector<Event> events;

	void start(const QVector2D& from, float safetyDepth)
	{
	}

	void end(const QVector2D& to, float safetyDepth)
	{
	}

	void startOperation(const QVector2D& to, float intensity)
	{
		events.push_back({Event::Type::StartOperation, to, 0.0f});
	}

	void endOperation(float safetyDepth)
	{
		events.push_back({Event::Type::EndOperation, QVector2D(), 0.0f});
	}

	void processPathAtDepth(const geometry::Polyline& polyline, float depth, float planeFeedRate, float depthFeedRate)
	{
		events.push_back({Event::Type::Path, polyline.start(), depth});
	}
};

class ExporterRendererFixture : public ExporterFixture
{
protected:
	using Event = EventTrackerVisitor::Event;

	config::Tools::Tool m_dragKnifeTool = m_tool;
	config::Profiles::Profile m_dragKnifeProfile = m_profile;

	void SetUp() override
	{
		m_dragKnifeTool.general().depthPerCut() = 0.05f;
		m_dragKnifeTool.dragKnife().alignmentDepth() = 0.02f;
		m_dragKnifeProfile.cut().passAtZeroDepth() = false;
		m_dragKnifeProfile.cut().direction() = geometry::CuttingDirection::FORWARD;
	}

	// Start of the entry, its shape is covered by the drag knife tests.
	QVector2D m_entryStart;

	std::vector<Event> renderDragKnife(geometry::Polyline &&polyline)
	{
		// Blade offset 0.5 and waste on the left.
		const geometry::tooloperation::DragKnife dragKnife(0.5f, 0.1f, qDegreesToRadians(0.1f), qDegreesToRadians(1.0f),
			geometry::CuttingDirection::FORWARD, geometry::CuttingDirection::FORWARD);
		m_entryStart = dragKnife.generate(polyline).polylines.front().entry->start();

		createTaskFromPolyline(std::move(polyline));

		m_task->forEachPath([&dragKnife](model::Path& path){
			path.createToolPath(dragKnife);
		});

		EventTrackerVisitor visitor;
		exporter::renderer::Renderer renderer(m_dragKnifeTool, m_dragKnifeProfile, visitor);
		renderer.render(*m_document);

		return visitor.events;
	}

	static void ExpectEvent(const Event& event, Event::Type type, const QVector2D& point = QVector2D(), float depth = 0.0f)
	{
		EXPECT_EQ(event.type, type);
		EXPECT_NEAR(event.point.x(), point.x(), 1e-5f);
		EXPECT_NEAR(event.point.y(), point.y(), 1e-5f);
		EXPECT_FLOAT_EQ(event.depth, depth);
	}
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

	// 0, 0.02, ..., 0.1
	const int nbCut = 6;
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

	// 0.02, ..., 0.1
	const int nbCut = 5;
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


TEST_F(ExporterRendererFixture, shouldAlignOnceBeforeClosedPolylinePasses)
{
	// Clockwise, so the waste on the left is outside.
	geometry::Polyline polyline({geometry::Bulge(QVector2D(0, 0), QVector2D(0, 10), 0),
		geometry::Bulge(QVector2D(0, 10), QVector2D(10, 0), 0), geometry::Bulge(QVector2D(10, 0), QVector2D(0, 0), 0)});

	const std::vector<Event> events = renderDragKnife(std::move(polyline));

	ASSERT_EQ(events.size(), 5);
	ExpectEvent(events[0], Event::Type::StartOperation, m_entryStart);
	ExpectEvent(events[1], Event::Type::Path, m_entryStart, -0.02f);
	ExpectEvent(events[2], Event::Type::Path, QVector2D(0, 0.5), -0.05f);
	ExpectEvent(events[3], Event::Type::Path, QVector2D(0, 0.5), -0.1f);
	ExpectEvent(events[4], Event::Type::EndOperation);
}

TEST_F(ExporterRendererFixture, shouldAlignOnceInsideCircle)
{
	// Counter clockwise, so the waste on the left is inside.
	geometry::Polyline polyline({geometry::Bulge(QVector2D(10, 0), QVector2D(-10, 0), 1),
		geometry::Bulge(QVector2D(-10, 0), QVector2D(10, 0), 1)});

	const std::vector<Event> events = renderDragKnife(std::move(polyline));

	ASSERT_EQ(events.size(), 5);
	ExpectEvent(events[0], Event::Type::StartOperation, m_entryStart);
	ExpectEvent(events[1], Event::Type::Path, m_entryStart, -0.02f);
	ExpectEvent(events[2], Event::Type::Path, QVector2D(10, 0.5), -0.05f);
	ExpectEvent(events[3], Event::Type::Path, QVector2D(10, 0.5), -0.1f);
	ExpectEvent(events[4], Event::Type::EndOperation);
}

TEST_F(ExporterRendererFixture, shouldReenterOpenPolylineForEachPass)
{
	geometry::Polyline polyline({geometry::Bulge(QVector2D(0, 0), QVector2D(10, 0), 0)});

	const std::vector<Event> events = renderDragKnife(std::move(polyline));

	ASSERT_EQ(events.size(), 8);
	ExpectEvent(events[0], Event::Type::StartOperation, m_entryStart);
	ExpectEvent(events[1], Event::Type::Path, m_entryStart, -0.02f);
	ExpectEvent(events[2], Event::Type::Path, QVector2D(0.5, 0), -0.05f);
	ExpectEvent(events[3], Event::Type::EndOperation);
	ExpectEvent(events[4], Event::Type::StartOperation, m_entryStart);
	ExpectEvent(events[5], Event::Type::Path, m_entryStart, -0.02f);
	ExpectEvent(events[6], Event::Type::Path, QVector2D(0.5, 0), -0.1f);
	ExpectEvent(events[7], Event::Type::EndOperation);
}

TEST_F(ExporterRendererFixture, shouldAlignAtFixedDepthEvenWithZeroDepthPass)
{
	m_dragKnifeProfile.cut().passAtZeroDepth() = true;

	geometry::Polyline polyline({geometry::Bulge(QVector2D(0, 0), QVector2D(10, 0), 0)});

	const std::vector<Event> events = renderDragKnife(std::move(polyline));

	ASSERT_GE(events.size(), 3);
	ExpectEvent(events[1], Event::Type::Path, m_entryStart, -0.02f);
	ExpectEvent(events[2], Event::Type::Path, QVector2D(0.5, 0), 0.0f);
}

TEST_F(ExporterRendererFixture, shouldShiftPassesByDepthOffset)
{
	const geometry::Bulge bulge(QVector2D(0, 0), QVector2D(1, 1), 0);
	geometry::Polyline polyline({bulge});

	createTaskFromPolyline(std::move(polyline));
	m_task->forEachPath([](model::Path& path) { path.settings().setDepthOffset(0.5f); });

	config::Tools::Tool tool = m_tool;
	tool.general().depthPerCut() = 0.02f;

	DepthTrackerVisitor visitor;
	exporter::renderer::Renderer renderer(tool, m_profile, visitor);
	renderer.render(*m_document);

	EXPECT_EQ(visitor.depths.size(), 5);
	EXPECT_FLOAT_EQ(visitor.depths.front(), -0.52f);
	EXPECT_FLOAT_EQ(visitor.depths.back(), -0.6f);
}

TEST_F(ExporterRendererFixture, shouldNotShiftDragKnifeEntryByDepthOffset)
{
	geometry::Polyline polyline({geometry::Bulge(QVector2D(0, 0), QVector2D(10, 0), 0)});

	const geometry::tooloperation::DragKnife dragKnife(0.5f, 0.1f, qDegreesToRadians(0.1f), qDegreesToRadians(1.0f),
		geometry::CuttingDirection::FORWARD, geometry::CuttingDirection::FORWARD);
	createTaskFromPolyline(std::move(polyline));
	m_task->forEachPath([&dragKnife](model::Path& path) {
		path.settings().setDepthOffset(0.5f);
		path.createToolPath(dragKnife);
	});

	EventTrackerVisitor visitor;
	exporter::renderer::Renderer renderer(m_dragKnifeTool, m_dragKnifeProfile, visitor);
	renderer.render(*m_document);

	ASSERT_EQ(visitor.events.size(), 8);
	EXPECT_FLOAT_EQ(visitor.events[1].depth, -0.02f);
	EXPECT_FLOAT_EQ(visitor.events[2].depth, -0.55f);
	EXPECT_FLOAT_EQ(visitor.events[5].depth, -0.02f);
	EXPECT_FLOAT_EQ(visitor.events[6].depth, -0.6f);
}

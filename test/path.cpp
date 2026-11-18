#include <gtest/gtest.h>

#include <cmath>
#include <QSignalSpy>
#include <QVector2D>

import geometry.bulge;
import geometry.circle;
import geometry.polyline;
import geometry.utils;
import model.path;
import model.pathgroupsettings;
import model.pathsettings;

TEST(PathTest, ShouldEmitSignalsWhenSelected)
{
	model::Path path(geometry::Polyline(), "1", model::PathSettings(1, 1, 1, 1));

	QSignalSpy spy(&path, &model::Path::selectedChanged);

	path.setSelected(true);

	ASSERT_EQ(spy.count(), 1);
	EXPECT_TRUE(spy.takeFirst().at(0).toBool());
}

TEST(PathTest, ShouldEmitSignalsWhenDeselected)
{
	model::Path path(geometry::Polyline(), "1", model::PathSettings(1, 1, 1, 1));
	path.setSelected(true);

	QSignalSpy spy(&path, &model::Path::selectedChanged);

	path.setSelected(false);

	ASSERT_EQ(spy.count(), 1);
	EXPECT_FALSE(spy.takeFirst().at(0).toBool());
}

TEST(PathTest, ShouldNoEmitSignalsWhenReselected)
{
	model::Path path(geometry::Polyline(), "1", model::PathSettings(1, 1, 1, 1));
	path.setSelected(true);

	QSignalSpy spy(&path, &model::Path::selectedChanged);

	path.setSelected(true);

	ASSERT_EQ(spy.count(), 0);
}

namespace
{

constexpr float bladeOffset = 0.5f;
constexpr float smallSwivelAngle = 0.1f;
const float quarterTurnBulge = std::tan(M_PI / 8.0);

void ExpectVectorNear(const QVector2D& actual, const QVector2D& expected)
{
	EXPECT_NEAR(actual.x(), expected.x(), 1e-5f);
	EXPECT_NEAR(actual.y(), expected.y(), 1e-5f);
}

void ExpectBulgeNear(const geometry::Bulge& actual, const QVector2D& start, const QVector2D& end, float tangent)
{
	ExpectVectorNear(actual.start(), start);
	ExpectVectorNear(actual.end(), end);
	EXPECT_NEAR(actual.tangent(), tangent, 1e-5f);
}

model::Path CreatePath(geometry::Bulge::List&& bulges)
{
	return model::Path(geometry::Polyline(std::move(bulges)), "1", model::PathSettings(1, 1, 1, 1));
}

geometry::Bulge::List Bulges(const geometry::Polyline& polyline)
{
	geometry::Bulge::List bulges;
	polyline.forEachBulge([&bulges](const geometry::Bulge& bulge) {
		bulges.push_back(bulge);
	});
	return bulges;
}

geometry::Bulge::List CompensatedBulges(const model::Path& path, float minimumSwivelAngle,
										 geometry::CuttingDirection direction = geometry::CuttingDirection::FORWARD)
{
	const geometry::Polyline::List polylines = path.dragKnifeCompensated(bladeOffset, minimumSwivelAngle, direction);
	EXPECT_EQ(polylines.size(), 1);
	return Bulges(polylines.front());
}

model::Path CreateLPath()
{
	return CreatePath({ geometry::Bulge(QVector2D(0, 0), QVector2D(10, 0), 0.0f),
						geometry::Bulge(QVector2D(10, 0), QVector2D(10, 10), 0.0f) });
}

}

TEST(PathTest, DragKnifeLineIsShiftedForward)
{
	const model::Path path = CreatePath({ geometry::Bulge(QVector2D(0, 0), QVector2D(10, 0), 0.0f) });

	const geometry::Bulge::List bulges = CompensatedBulges(path, smallSwivelAngle);

	ASSERT_EQ(bulges.size(), 1);
	ExpectBulgeNear(bulges[0], QVector2D(0.5, 0), QVector2D(10.5, 0), 0.0f);
}

TEST(PathTest, DragKnifeCornerAddsSwivelAroundVertex)
{
	const geometry::Bulge::List bulges = CompensatedBulges(CreateLPath(), smallSwivelAngle);

	ASSERT_EQ(bulges.size(), 3);
	ExpectBulgeNear(bulges[0], QVector2D(0.5, 0), QVector2D(10.5, 0), 0.0f);
	ExpectBulgeNear(bulges[1], QVector2D(10.5, 0), QVector2D(10, 0.5), quarterTurnBulge);
	ExpectBulgeNear(bulges[2], QVector2D(10, 0.5), QVector2D(10, 10.5), 0.0f);
}

TEST(PathTest, DragKnifeCornerBelowMinimumSwivelAngleJoinsBulges)
{
	const geometry::Bulge::List bulges = CompensatedBulges(CreateLPath(), M_PI_2 + 0.1f);

	ASSERT_EQ(bulges.size(), 2);
	ExpectBulgeNear(bulges[0], QVector2D(0.5, 0), QVector2D(10.5, 0), 0.0f);
	ExpectBulgeNear(bulges[1], QVector2D(10.5, 0), QVector2D(10, 10.5), 0.0f);
}

TEST(PathTest, DragKnifeBackwardIsCompensatedInCuttingDirection)
{
	const geometry::Bulge::List bulges
		= CompensatedBulges(CreateLPath(), smallSwivelAngle, geometry::CuttingDirection::BACKWARD);

	ASSERT_EQ(bulges.size(), 3);
	ExpectBulgeNear(bulges[0], QVector2D(10, 9.5), QVector2D(10, -0.5), 0.0f);
	ExpectBulgeNear(bulges[1], QVector2D(10, -0.5), QVector2D(9.5, 0), -quarterTurnBulge);
	ExpectBulgeNear(bulges[2], QVector2D(9.5, 0), QVector2D(-0.5, 0), 0.0f);
}

TEST(PathTest, DragKnifeArcKeepsCenterAndSweep)
{
	const model::Path path = CreatePath({ geometry::Bulge(QVector2D(1, 0), QVector2D(0, 1), quarterTurnBulge) });

	const geometry::Bulge::List bulges = CompensatedBulges(path, smallSwivelAngle);

	ASSERT_EQ(bulges.size(), 1);
	ExpectBulgeNear(bulges[0], QVector2D(1, 0.5), QVector2D(-0.5, 1), quarterTurnBulge);

	const geometry::Circle circle = bulges[0].toCircle();
	ExpectVectorNear(circle.center(), QVector2D(0, 0));
	EXPECT_NEAR(circle.radius(), std::sqrt(1.0f + bladeOffset * bladeOffset), 1e-5f);
}

TEST(PathTest, DragKnifeClosedPolylineSwivelsAtClosingVertex)
{
	const model::Path path = CreatePath({ geometry::Bulge(QVector2D(0, 0), QVector2D(10, 0), 0.0f),
										  geometry::Bulge(QVector2D(10, 0), QVector2D(10, 10), 0.0f),
										  geometry::Bulge(QVector2D(10, 10), QVector2D(0, 10), 0.0f),
										  geometry::Bulge(QVector2D(0, 10), QVector2D(0, 0), 0.0f) });

	const geometry::Polyline::List polylines
		= path.dragKnifeCompensated(bladeOffset, smallSwivelAngle, geometry::CuttingDirection::FORWARD);
	ASSERT_EQ(polylines.size(), 1);
	EXPECT_TRUE(polylines.front().isClosed());

	const geometry::Bulge::List bulges = Bulges(polylines.front());
	ASSERT_EQ(bulges.size(), 8);
	ExpectBulgeNear(bulges[7], QVector2D(0, -0.5), QVector2D(0.5, 0), quarterTurnBulge);
}

TEST(PathTest, DragKnifeTangentContinuousBulgesHaveNoSwivel)
{
	const model::Path path = CreatePath({ geometry::Bulge(QVector2D(0, 0), QVector2D(1, 0), 0.0f),
										  geometry::Bulge(QVector2D(1, 0), QVector2D(2, 1), quarterTurnBulge) });

	const geometry::Bulge::List bulges = CompensatedBulges(path, 0.0f);

	ASSERT_EQ(bulges.size(), 2);
	ExpectBulgeNear(bulges[0], QVector2D(0.5, 0), QVector2D(1.5, 0), 0.0f);
	ExpectBulgeNear(bulges[1], QVector2D(1.5, 0), QVector2D(2, 1.5), quarterTurnBulge);
}

TEST(PathTest, DragKnifePointIsUnchanged)
{
	const model::Path path = CreatePath({ geometry::Bulge(QVector2D(1, 2), QVector2D(1, 2), 0.0f) });

	const geometry::Bulge::List bulges = CompensatedBulges(path, smallSwivelAngle);

	ASSERT_EQ(bulges.size(), 1);
	ExpectBulgeNear(bulges[0], QVector2D(1, 2), QVector2D(1, 2), 0.0f);
}

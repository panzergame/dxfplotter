#include <gtest/gtest.h>

#include <cmath>
#include <QSignalSpy>
#include <QVector2D>

import geometry.bulge;
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

geometry::Bulge::List CompensatedBulges(geometry::CuttingDirection direction)
{
	const model::Path path(geometry::Polyline({ geometry::Bulge(QVector2D(0, 0), QVector2D(10, 0), 0.0f),
												geometry::Bulge(QVector2D(10, 0), QVector2D(10, 10), 0.0f) }),
						   "1", model::PathSettings(1, 1, 1, 1));

	const geometry::Polyline::List polylines = path.dragKnifeCompensated(bladeOffset, smallSwivelAngle, direction);
	EXPECT_EQ(polylines.size(), 1);

	geometry::Bulge::List bulges;
	polylines.front().forEachBulge([&bulges](const geometry::Bulge& bulge) {
		bulges.push_back(bulge);
	});
	return bulges;
}

}

TEST(PathTest, DragKnifeForwardIsCompensatedInPolylineDirection)
{
	const geometry::Bulge::List bulges = CompensatedBulges(geometry::CuttingDirection::FORWARD);

	ASSERT_EQ(bulges.size(), 3);
	ExpectBulgeNear(bulges[0], QVector2D(0.5, 0), QVector2D(10.5, 0), 0.0f);
	ExpectBulgeNear(bulges[1], QVector2D(10.5, 0), QVector2D(10, 0.5), quarterTurnBulge);
	ExpectBulgeNear(bulges[2], QVector2D(10, 0.5), QVector2D(10, 10.5), 0.0f);
}

TEST(PathTest, DragKnifeBackwardIsCompensatedInCuttingDirection)
{
	const geometry::Bulge::List bulges = CompensatedBulges(geometry::CuttingDirection::BACKWARD);

	ASSERT_EQ(bulges.size(), 3);
	ExpectBulgeNear(bulges[0], QVector2D(10, 9.5), QVector2D(10, -0.5), 0.0f);
	ExpectBulgeNear(bulges[1], QVector2D(10, -0.5), QVector2D(9.5, 0), -quarterTurnBulge);
	ExpectBulgeNear(bulges[2], QVector2D(9.5, 0), QVector2D(-0.5, 0), 0.0f);
}

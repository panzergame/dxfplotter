#include <gtest/gtest.h>
#include <model/pathgroupsettings.h>

#include <QSignalSpy>

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

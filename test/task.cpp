#include <gtest/gtest.h>
#include <model/pathgroupsettings.h>

#include <QSignalSpy>

TEST(TaskTest, ShouldEmitSignalsWhenOnePathSelected)
{
	model::Path::UPtr path1 = std::make_unique<model::Path>(geometry::Polyline(), "1", model::PathSettings(1, 1, 1, 1));
	model::Path::UPtr path2 = std::make_unique<model::Path>(geometry::Polyline(), "2", model::PathSettings(2, 2, 2, 2));

	model::Path::ListUPtr paths;
	paths.push_back(std::move(path1));
	paths.push_back(std::move(path2));
	model::Layer::UPtr layer = std::make_unique<model::Layer>("layer", std::move(paths));

	model::Layer::ListUPtr layers;
	layers.push_back(std::move(layer));
	model::Task task(std::move(layers));

	QSignalSpy spy(&task, &model::Task::selectionChanged);

	model::Path &firstPath = task.pathAt(0);
	firstPath.setSelected(true);

	ASSERT_EQ(spy.count(), 1);
	EXPECT_FALSE(spy.takeFirst().at(0).toBool());
}

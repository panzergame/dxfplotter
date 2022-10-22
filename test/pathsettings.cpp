#include <gtest/gtest.h>
#include <model/pathgroupsettings.h>

TEST(PathGroupSettingsTest, ShouldShareValueAfterSet)
{
	model::Path::UPtr path1 = std::make_unique<model::Path>(geometry::Polyline(), "1", model::PathSettings(1, 1, 1, 1));
	model::Path::UPtr path2 = std::make_unique<model::Path>(geometry::Polyline(), "2", model::PathSettings(2, 2, 2, 2));
	
	path1->setSelected(true);
	path2->setSelected(true);

	model::Path::ListUPtr paths;
	paths.push_back(std::move(path1));
	paths.push_back(std::move(path2));
	model::Layer::UPtr layer = std::make_unique<model::Layer>("layer", std::move(paths));

	model::Layer::ListUPtr layers;
	layers.push_back(std::move(layer));
	model::Task task(std::move(layers));

	model::PathGroupSettings group(task);

	EXPECT_EQ(std::nullopt, group.planeFeedRate());
	EXPECT_EQ(std::nullopt, group.depthFeedRate());
	EXPECT_EQ(std::nullopt, group.intensity());
	EXPECT_EQ(std::nullopt, group.depth());

	group.setPlaneFeedRate(3);
	group.setDepthFeedRate(3);
	group.setIntensity(3);
	group.setDepth(3);

	EXPECT_EQ(3, group.planeFeedRate());
	EXPECT_EQ(3, group.depthFeedRate());
	EXPECT_EQ(3, group.intensity());
	EXPECT_EQ(3, group.depth());
}

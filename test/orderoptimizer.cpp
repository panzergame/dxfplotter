#include <gtest/gtest.h>

#include <geometry/orderoptimizer.h>

#include <QDebug>

TEST(OrderOptimizer, shouldRespectGroups)
{
	geometry::OrderOptimizer::NodesPerGroup groups = {
		{
			{{}, 1, {1, 4}},
			{{}, 3, {1, 2}},
			{{}, 2, {2, 3}}
		},
		{
			{{}, 0, {4, 1}},
			{{}, 5, {5, 3}}
		},
		{
			{{}, 4, {2, 2}}
		}
	};

	std::vector<int> groupById(6);
	for (int groupId = 0, nbGroup = groups.size(); groupId < nbGroup; ++groupId) {
		for (const geometry::OrderOptimizer::Node& node : groups[groupId]) {
			groupById[node.id] = groupId;
		}
	}

	geometry::OrderOptimizer optimizer(groups, 6);
	const std::vector<int> order = optimizer.order();

	for (int id : order) {
		std::cout << id << " ";
	}
	std::cout << std::endl;

	for (int i = 0; i < 5; ++i) {
		const int group1 = groupById[order[i]];
		const int group2 = groupById[order[i + 1]];
		EXPECT_LE(group1, group2);
	}
}


TEST(OrderOptimizer, shouldOrderSingleGroup)
{
	geometry::OrderOptimizer::NodesPerGroup groups = {
		{
			{{}, 0, {1, 4}},
			{{}, 1, {1, 2}},
			{{}, 2, {2, 3}},
			{{}, 3, {4, 1}}
		}
	};

	geometry::OrderOptimizer optimizer(groups, 4);
	const std::vector<int> order = optimizer.order();
}

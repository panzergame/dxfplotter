module;

#include <vector>
#include <QVector2D>

export module geometry.orderoptimizer;

import common.aggregable;

export namespace geometry
{

class OrderOptimizer
{
public:
	struct Node : common::Aggregable<Node>
	{
		int id;
		QVector2D position;

		inline bool operator==(const Node& other) const { return id == other.id; }
	};

	using NodesPerGroup = std::vector<Node::List>;

private:
	std::vector<int> m_order;

public:
	explicit OrderOptimizer(const NodesPerGroup& nodesPerGroup, int nbNodes);

	const std::vector<int>& order() const { return m_order; }
};

}

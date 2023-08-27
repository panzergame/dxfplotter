#pragma once

#include <common/aggregable.h>
#include <QVector2D>

#include "ortools/sat/cp_model.h"

namespace ortools = operations_research::sat;

namespace geometry
{

class OrderOptimizer
{
public:
	struct Node : common::Aggregable<Node>
	{
		int id;
		QVector2D position;

		inline bool operator==(const Node& other) const
		{
			return id == other.id;
		}
	};

	using NodesPerGroup = std::vector<Node::List>;

private:
	std::vector<int> m_order;

	static int nodeDistance(const Node& n1, const Node& n2);

	struct ArcTo : common::Aggregable<ArcTo>
	{
		int target;
		ortools::BoolVar literal;
	};

	using ArcsByNodes = std::vector<ArcTo::List>;

	struct Model
	{
		ortools::CpModelProto proto;
		ArcsByNodes arcsByNodes;
		int nbNodes;
	};

	struct ModelBuilder
	{
		ortools::CpModelBuilder builder;
		ortools::CircuitConstraint circuit;
		ArcsByNodes arcsByNodes;
		ortools::LinearExpr pathLength;
		int nbNodes;

		explicit ModelBuilder(int nbNodes);

		void addArc(const Node& n1, const Node& n2);
		Model build();
	};

	Model buildModel(const NodesPerGroup& nodesPerGroup, int nbNodes) const;
	std::vector<int> solveAndExtractOrder(const Model& model);

public:
	explicit OrderOptimizer(const NodesPerGroup& nodesPerGroup, int nbNodes);

	const std::vector<int> &order() const;
};

}

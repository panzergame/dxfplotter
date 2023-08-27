#include <orderoptimizer.h>

#include <QDebug> // TODO

namespace geometry
{

int OrderOptimizer::nodeDistance(const Node& n1, const Node& n2)
{
	return (n1.position - n2.position).lengthSquared() * 1e4; // TODO mutiplier
}

static int homeNodeId(int nbNodes)
{
	return nbNodes;
}

OrderOptimizer::ModelBuilder::ModelBuilder(int nbNodes)
	:circuit(builder.AddCircuitConstraint()),
	arcsByNodes(nbNodes + 1),
	nbNodes(nbNodes)
{
}

void OrderOptimizer::ModelBuilder::addArc(const Node& n1, const Node& n2)
{
	ortools::BoolVar literal = builder.NewBoolVar();
	circuit.AddArc(n1.id, n2.id, literal);

	const int distance = nodeDistance(n1, n2);
	pathLength += literal * distance;

	arcsByNodes[n1.id].push_back({{}, n2.id, literal});
}

geometry::OrderOptimizer::Model OrderOptimizer::ModelBuilder::build()
{
	builder.Minimize(pathLength);

	return {builder.Build(), arcsByNodes, nbNodes};
}

OrderOptimizer::Model OrderOptimizer::buildModel(const NodesPerGroup& nodesPerGroup, int nbNodes) const
{
	ModelBuilder modelBuilder(nbNodes);

	const int nbGroup = nodesPerGroup.size();
	const int lastGroupId = nbGroup - 1;

	// Group intra connections
	for (const Node::List& nodes : nodesPerGroup) {
		for (const Node& n1 : nodes) {
			for (const Node& n2 : nodes) {
				if (n1 == n2) {
					continue;
				}

				modelBuilder.addArc(n1, n2);
			}
		}
	}

	// Group inter connections with group of id + 1
	for (int groupId = 0; groupId < lastGroupId; ++groupId) {
		const Node::List &curNodes = nodesPerGroup[groupId];
		const Node::List &nextNodes = nodesPerGroup[groupId + 1];

		for (const Node& n1 : curNodes) {
			for (const Node& n2 : nextNodes) {
				modelBuilder.addArc(n1, n2);
			}
		}
	}

	const Node home{{}, homeNodeId(nbNodes), {0.0f, 0.0f}};

	// Home to first group nodes
	const Node::List &firstGroupsNodes = nodesPerGroup.front();
	for (const Node& node : firstGroupsNodes) {
		modelBuilder.addArc(home, node);
	}

	// Last group nodes to home
	const Node::List &lastGroupsNodes = nodesPerGroup.back();
	for (const Node& node : lastGroupsNodes) {
		modelBuilder.addArc(node, home);
	}

	return modelBuilder.build();
}

std::vector<int> OrderOptimizer::solveAndExtractOrder(const Model& model)
{
	const ortools::CpSolverResponse response = ortools::Solve(model.proto);

	if (response.status() != ortools::CpSolverStatus::OPTIMAL && response.status() != ortools::CpSolverStatus::FEASIBLE) {
		return {};
	}

	std::vector<int> order;

	const ArcsByNodes arcsByNodes = model.arcsByNodes;
	// Last node id is home
	const int homeId = homeNodeId(model.nbNodes);
	int curNodeId = homeId;
	while (order.size() < homeId) {
		for (const ArcTo& arc : arcsByNodes[curNodeId]) {
			if (ortools::SolutionIntegerValue(response, arc.literal)) {
				curNodeId = arc.target;
				order.push_back(curNodeId);
				continue;
			}
		}
	}

	return order;
}

OrderOptimizer::OrderOptimizer(const NodesPerGroup& nodesPerGroup, int nbNodes)
{
	const Model model = buildModel(nodesPerGroup, nbNodes);
	m_order = solveAndExtractOrder(model);
}

const std::vector<int> &OrderOptimizer::order() const
{
	return m_order;
}


}

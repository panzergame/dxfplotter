#include <scene.h>

#include <QWidget>

namespace view::simulation::internal
{

Scene::Scene(const model::Simulation& simulation)
	:m_tool(this, simulation.toolRadius(), 1.0f),
	m_toolPath(this, simulation.approximatedToolPathToLines(0.01))
{
}

void Scene::setToolPosition(const model::Simulation::ToolPathPoint3D& position)
{
	m_tool.setPosition(position);
}

}


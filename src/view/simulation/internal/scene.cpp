module;

#include <QWidget>
#include <QEntity>

export module view.simulation.internal.scene;

import view.simulation.internal.tool;
import view.simulation.internal.toolpath;
import model.simulation;

export namespace view::simulation::internal
{

class Scene : public Qt3DCore::QEntity
{
private:
	Tool m_tool;
	ToolPath m_toolPath;

public:
	explicit Scene(const model::Simulation& simulation);

	void setToolPosition(const model::Simulation::ToolPathPoint3D& position);
};

}

namespace view::simulation::internal
{

Scene::Scene(const model::Simulation& simulation)
	: m_tool(this, simulation.toolRadius(), 1.0f)
	, m_toolPath(this, simulation.approximatedToolPathToLines(0.01))
{
}

void Scene::setToolPosition(const model::Simulation::ToolPathPoint3D& position)
{
	m_tool.setPosition(position);
}

}

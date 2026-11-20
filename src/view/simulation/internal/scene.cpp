module;

#include <QWidget>
#include <Qt3DCore/QEntity>
#include <memory>

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
	std::unique_ptr<ToolPath> m_toolPath;

public:
	explicit Scene()
		: m_tool(this, 1.0f)
	{
	}

	// Entities are updated in place rather than the whole scene being recreated: Qt3D RHI renderer
	// crashes when tool and tool path entities (with different materials) are created in the same frame.
	void setSimulation(const model::Simulation& simulation)
	{
		m_tool.setRadius(simulation.toolRadius());
		m_toolPath = std::make_unique<ToolPath>(this, simulation.approximatedToolPathToLines(0.01));
	}

	void setToolPosition(const model::Simulation::ToolPathPoint3D& position) { m_tool.setPosition(position); }
};

}

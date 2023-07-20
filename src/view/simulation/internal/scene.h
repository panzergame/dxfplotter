#pragma once

#include <view/simulation/internal/tool.h>
#include <view/simulation/internal/toolpath.h>

namespace view::simulation::internal
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

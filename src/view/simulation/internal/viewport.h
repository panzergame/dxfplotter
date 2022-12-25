#pragma once

#include <QVTKOpenGLNativeWidget.h>

#include <view/simulation/internal/tool.h>
#include <view/simulation/internal/toolpath.h>

namespace view::simulation::internal
{

class Viewport : public QVTKOpenGLNativeWidget
{
private:
	Tool m_tool;
	ToolPath m_toolPath;

public:
	explicit Viewport(const model::Simulation& simulation);

	void setToolPosition(const model::Simulation::ToolPathPoint3D& position);
};

}

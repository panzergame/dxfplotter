#pragma once

#include <model/documentmodelobserver.h>

#include <QWidget>

#include <memory>

class QVTKOpenGLNativeWidget;

namespace view::view3d
{

namespace internal
{

class ToolPath;
class Viewport;

}

class Viewport : public model::DocumentModelObserver<QWidget>
{
private:
	model::Simulation m_simulation;
	QVTKOpenGLNativeWidget *m_vtkWidget;
	std::unique_ptr<internal::ToolPath> m_path;
	std::unique_ptr<internal::Viewport> m_viewport;

protected:
	void documentChanged() override;
	void resizeEvent(QResizeEvent *event) override;

public:
	explicit Viewport(model::Application &app);

	void setSimulation(model::Simulation&& simulation);
};

}

#pragma once

#include <Qt3DExtras/Qt3DWindow>

namespace view::simulation::internal
{

class Scene;

class Viewport : public Qt3DExtras::Qt3DWindow
{
private:

public:
	explicit Viewport();

	QWidget *container();

	void setScene(Scene *scene);
};

}

#pragma once

#include <ui_viewport.h>

#include <core/application.h>

#include <QGraphicsView>

namespace View
{

class Viewport : public QGraphicsView, private Ui::Viewport
{
	Core::Application &m_app;

public:
	explicit Viewport(Core::Application &app);
};

}

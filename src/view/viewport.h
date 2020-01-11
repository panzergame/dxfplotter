#pragma once

#include <ui_viewport.h>

#include <control/application.h>

#include <QGraphicsView>

namespace View
{

class Viewport : public QGraphicsView, private Ui::Viewport
{
	Control::Application &m_app;

public:
	explicit Viewport(Control::Application &app);
};

}

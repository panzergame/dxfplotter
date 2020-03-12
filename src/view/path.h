#pragma once

#include <ui_path.h>

#include <control/application.h>

#include <QWidget>

namespace View
{

class Path : public QWidget, private Ui::Path
{
private:
	Control::Application &m_app;

	void pathSelected(Model::Path *path);

public:
	explicit Path(Control::Application &app);

};

}

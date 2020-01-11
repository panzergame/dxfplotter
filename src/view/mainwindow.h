#pragma once

#include <control/application.h>

#include <ui_mainwindow.h>

#include <QMainWindow>

namespace View
{

class Task;
class Viewport;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
private:
	Control::Application &m_app;

	Task *m_task;
	Viewport *m_viewport;

public:
	explicit MainWindow(Control::Application &app);

public Q_SLOTS:
	void openFile();
};

}

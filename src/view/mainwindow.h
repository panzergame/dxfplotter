#pragma once

#include <core/application.h>

#include <ui_mainwindow.h>

#include <QMainWindow>

namespace View
{

class Task;
class Viewport;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
private:
	Core::Application &m_app;

	Task *m_task;
	Viewport *m_viewport;

public:
	explicit MainWindow(Core::Application &app);

public Q_SLOTS:
	void openFile();
};

}

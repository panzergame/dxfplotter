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

	QWidget *setupLeftPanel();
	QWidget *setupCenterPanel();
	void setupUi();

public:
	explicit MainWindow(Control::Application &app);

protected Q_SLOTS:
	void openFile();
};

}

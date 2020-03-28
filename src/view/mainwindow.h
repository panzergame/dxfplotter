#pragma once

#include <model/application.h>

#include <ui_mainwindow.h>

#include <QMainWindow>

namespace View
{

class Task;
class Viewport;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
private:
	Model::Application &m_app;

	QWidget *setupLeftPanel();
	QWidget *setupCenterPanel();
	void setupUi();

public:
	explicit MainWindow(Model::Application &app);

protected Q_SLOTS:
	void openFile();
	void exportFile();
	void openSettings();
};

}

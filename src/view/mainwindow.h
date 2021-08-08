#pragma once

#include <model/application.h>

#include <uic/ui_mainwindow.h>

#include <QMainWindow>

class QComboBox;

namespace View
{

namespace Task
{

class Task;
class Viewport;

}

class MainWindow : public QMainWindow, private Ui::MainWindow
{
private:
	Model::Application &m_app;

	QWidget *setupLeftPanel();
	QWidget *setupCenterPanel();
	void setupToolBar();
	void setupUi();
	void setupMenuActions();

	void setTaskToolsEnabled(bool enabled);

public:
	explicit MainWindow(Model::Application &app);

protected Q_SLOTS:
	void openFile();
	void exportFile();
	void openSettings();
	void taskChanged(Model::Task *newTask);
	void displayError(const QString &message);
};

}

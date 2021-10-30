#pragma once

#include <model/application.h>

#include <uic/ui_mainwindow.h>

#include <QMainWindow>

class QComboBox;


namespace view
{

namespace Task
{

class Task;
class Viewport;

}

class MainWindow : public QMainWindow, private Ui::MainWindow
{
private:
	model::Application &m_app;

	QWidget *setupLeftPanel();
	QWidget *setupCenterPanel();
	void setupToolBar();
	void setupUi();
	void setupMenuActions();

	void setTaskToolsEnabled(bool enabled);

public:
	explicit MainWindow(model::Application &app);

protected Q_SLOTS:
	void openFile();
	void saveFile();
	void saveAsFile();
	void exportFile();
	void openSettings();
	void transformSelection();
	void documentChanged(model::Document *newDocument);
	void displayError(const QString &message);
};

}

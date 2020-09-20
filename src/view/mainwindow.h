#pragma once

#include <model/application.h>

#include <uic/ui_mainwindow.h>

#include <QMainWindow>

class QComboBox;

namespace View
{

class Task;
class Viewport;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
private:
	Model::Application &m_app;
	QComboBox *m_toolSelector;

	QWidget *setupLeftPanel();
	QWidget *setupCenterPanel();
	void setupToolBar();
	void setupUi();
	void setupMenuActions();
	void updateToolSelector(const Config::Config &config);

public:
	explicit MainWindow(Model::Application &app);

protected Q_SLOTS:
	void openFile();
	void exportFile();
	void openSettings();
	void configChanged(const Config::Config &config);
};

}

#include <mainwindow.h>
#include <info.h>
#include <profile.h>
#include <task/path.h>
#include <task/task.h>
#include <view2d/viewport.h>
#include <settings/settings.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QSplitter>
#include <QComboBox>
#include <QDebug>

namespace View
{

QWidget *MainWindow::setupLeftPanel()
{
	Task::Task *task = new Task::Task(m_app);
	Task::Path *path = new Task::Path(m_app);

	QSplitter *vertSplitter = new QSplitter(Qt::Vertical, this);
	vertSplitter->addWidget(task);
	vertSplitter->addWidget(path);
	vertSplitter->setStretchFactor(0, 1);
	vertSplitter->setStretchFactor(1, 0);

	return vertSplitter;
}

QWidget *MainWindow::setupCenterPanel()
{
	View2d::Viewport *viewport = new View2d::Viewport(m_app);
	Info *info = new Info(viewport);

	QWidget *container = new QWidget(this);
	QVBoxLayout *layout = new QVBoxLayout();

	layout->addWidget(viewport);
	layout->addWidget(info);

	container->setLayout(layout);

	return container;
}

void MainWindow::setupToolBar()
{
	Profile *profileBar = new Profile(m_app);
	toolBar->addWidget(profileBar);
}

void MainWindow::setupUi()
{
	Ui::MainWindow::setupUi(this);

	QSplitter *horiSplitter = new QSplitter(Qt::Horizontal, this);
	horiSplitter->addWidget(setupLeftPanel());
	horiSplitter->addWidget(setupCenterPanel());
	horiSplitter->setStretchFactor(0, 0);
	horiSplitter->setStretchFactor(1, 1);

	horizontalLayout->addWidget(horiSplitter);

	setupToolBar();
}

void MainWindow::setupMenuActions()
{
	// File actions
	connect(actionOpenFile, &QAction::triggered, this, &MainWindow::openFile);
	connect(actionExportFile, &QAction::triggered, this, &MainWindow::exportFile);
	connect(actionOpenSettings, &QAction::triggered, this, &MainWindow::openSettings);

	// Edit actions
	connect(actionLeftCutterCompensation, &QAction::triggered, &m_app, &Model::Application::leftCutterCompensation);
	connect(actionRightCutterCompensation, &QAction::triggered, &m_app, &Model::Application::rightCutterCompensation);
	connect(actionResetCutterCompensation, &QAction::triggered, &m_app, &Model::Application::resetCutterCompensation);
	connect(actionHideSelection, &QAction::triggered, &m_app, &Model::Application::hideSelection);
	connect(actionShowHidden, &QAction::triggered, &m_app, &Model::Application::showHidden);
}

MainWindow::MainWindow(Model::Application &app)
	:m_app(app)
{
	setupUi();
	setupMenuActions();
	showMaximized();

	connect(&m_app, &Model::Application::titleChanged, this, &MainWindow::setWindowTitle);
}

void MainWindow::openFile()
{
	const QString fileName = QFileDialog::getOpenFileName(this);
	if (!fileName.isEmpty()) {
		if (!m_app.loadFile(fileName)) {
			QMessageBox messageBox;
			messageBox.critical(this, "Error", "Invalid file type " + fileName);
		}
	}
}

void MainWindow::exportFile()
{
	const QString defaultPath = m_app.currentFileBaseName() + ".ngc";
	const QString fileName = QFileDialog::getSaveFileName(this, "Export File", defaultPath, "Text files (*.ngc *.txt)");

	if (!fileName.isEmpty()) {
		if (!m_app.exportToGcode(fileName)) {
			QMessageBox messageBox;
			messageBox.critical(this, "Error", "Couldn't save " + fileName);
		}
	}
}

void MainWindow::openSettings()
{
	Settings::Settings settings(m_app);
	if (settings.exec() == QDialog::Accepted) {
		m_app.setConfig(settings.newConfig());
	}
}

}

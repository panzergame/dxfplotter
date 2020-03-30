#include <mainwindow.h>
#include <path.h>
#include <task.h>
#include <viewport.h>
#include <settings.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QSplitter>
#include <QDebug>

namespace View
{

QWidget *MainWindow::setupLeftPanel()
{
	Task *task = new Task(m_app);
	Path *path = new Path(m_app);

	QSplitter *vertSplitter = new QSplitter(Qt::Vertical, this);
	vertSplitter->addWidget(task);
	vertSplitter->addWidget(path);
	vertSplitter->setStretchFactor(0, 1);
	vertSplitter->setStretchFactor(1, 0);

	return vertSplitter;
}

QWidget *MainWindow::setupCenterPanel()
{
	Viewport *viewport = new Viewport(m_app);
	return viewport;
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
}

MainWindow::MainWindow(Model::Application &app)
	:m_app(app)
{
	setupUi();

	showMaximized();

	connect(actionOpenFile, &QAction::triggered, this, &MainWindow::openFile);
	connect(actionExportFile, &QAction::triggered, this, &MainWindow::exportFile);
	connect(actionOpenSettings, &QAction::triggered, this, &MainWindow::openSettings);

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
	const QString fileName = QFileDialog::getSaveFileName(this, "Export File", "", "Text files (*.ngc *.txt)");

	if (!fileName.isEmpty()) {
		if (!m_app.exportToGcode(fileName)) {
			QMessageBox messageBox;
			messageBox.critical(this, "Error", "Couldn't save " + fileName);
		}
	}
}

void MainWindow::openSettings()
{
	Settings *settings = new Settings(m_app.config());
	settings->exec();
}

}

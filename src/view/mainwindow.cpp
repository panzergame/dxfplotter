#include <mainwindow.h>
#include <info.h>
#include <path.h>
#include <task.h>
#include <viewport.h>
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
	// Tool selector
	m_toolSelector = new QComboBox();

	connect(m_toolSelector, &QComboBox::currentTextChanged, &m_app, &Model::Application::selectTool);

	toolBar->addWidget(m_toolSelector);
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
}

void MainWindow::updateToolSelector(const Config::Config &config)
{
	const Config::Tools &tools = config.root().tools();

	m_toolSelector->clear();

	tools.visitChildren([this](const auto &tool){
		const QString name = QString::fromStdString(tool.name());
		m_toolSelector->addItem(name, name);
	});
}

MainWindow::MainWindow(Model::Application &app)
	:m_app(app)
{
	setupUi();
	setupMenuActions();
	showMaximized();
	updateToolSelector(m_app.config());

	connect(&m_app, &Model::Application::titleChanged, this, &MainWindow::setWindowTitle);
	connect(&m_app, &Model::Application::configChanged, this, &MainWindow::configChanged);
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
	Settings::Settings *settings = new Settings::Settings(m_app);
	if (settings->exec() == QDialog::Accepted) {
		m_app.setConfig(settings->newConfig());
	}

	delete settings;
}

void MainWindow::configChanged(const Config::Config &config)
{
	updateToolSelector(config);
}


}

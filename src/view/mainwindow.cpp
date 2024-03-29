#include <mainwindow.h>
#include <info.h>
#include <profile.h>
#include <task/path.h>
#include <task/task.h>
#include <view2d/viewport.h>
#include <simulation/simulation.h>
#include <dialogs/settings/settings.h>
#include <dialogs/transform.h>
#include <dialogs/mirror.h>
#include <dialogs/setorigin.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QSplitter>
#include <QComboBox>
#include <QErrorMessage>
#include <QDebug>

namespace view
{

QWidget *MainWindow::setupLeftPanel()
{
	task::Task *task = new task::Task(m_app);
	task::Path *path = new task::Path(m_app);

	QSplitter *vertSplitter = new QSplitter(Qt::Vertical, this);
	vertSplitter->addWidget(task);
	vertSplitter->addWidget(path);
	vertSplitter->setStretchFactor(0, 1);
	vertSplitter->setStretchFactor(1, 0);

	return vertSplitter;
}

QWidget *MainWindow::setupCenterPanel()
{
	view2d::Viewport *viewport2d = new view2d::Viewport(m_app);
	m_simulation = new simulation::Simulation();
	Info *info = new Info(*viewport2d, m_app);

	QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
	splitter->addWidget(viewport2d);
	splitter->addWidget(m_simulation);
	splitter->setStretchFactor(0, 1);
	splitter->setStretchFactor(1, 0);

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(splitter);
	layout->addWidget(info);
	layout->setStretch(0, 1);

	QWidget *container = new QWidget(this);
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
	connect(actionSaveFile, &QAction::triggered, this, &MainWindow::saveFile);
	connect(actionSaveAsFile, &QAction::triggered, this, &MainWindow::saveAsFile);
	connect(actionExportFile, &QAction::triggered, this, &MainWindow::exportFile);
	connect(actionExportAsFile, &QAction::triggered, this, &MainWindow::exportAsFile);
	connect(actionOpenSettings, &QAction::triggered, this, &MainWindow::openSettings);

	// Edit actions
	connect(actionLeftCutterCompensation, &QAction::triggered, &m_app, &model::Application::leftCutterCompensation);
	connect(actionRightCutterCompensation, &QAction::triggered, &m_app, &model::Application::rightCutterCompensation);
	connect(actionResetCutterCompensation, &QAction::triggered, &m_app, &model::Application::resetCutterCompensation);
	connect(actionPocketSelection, &QAction::triggered, &m_app, &model::Application::pocketSelection);
	connect(actionHideSelection, &QAction::triggered, &m_app, &model::Application::hideSelection);
	connect(actionShowHidden, &QAction::triggered, &m_app, &model::Application::showHidden);
	connect(actionTransformSelection, &QAction::triggered, this, &MainWindow::transformSelection);
	connect(actionMirrorSelection, &QAction::triggered, this, &MainWindow::mirrorSelection);
	connect(actionSetSelectionOrigin, &QAction::triggered, this, &MainWindow::setSelectionOrigin);
	connect(actionSimulate, &QAction::triggered, this, &MainWindow::simulate);
}

void MainWindow::setupOpenedDocumentActions()
{
	m_openedDocumentActions.addAction(actionExportFile);
	m_openedDocumentActions.addAction(actionExportAsFile);
	m_openedDocumentActions.addAction(actionSaveFile);
	m_openedDocumentActions.addAction(actionSaveAsFile);
	m_openedDocumentActions.addAction(actionLeftCutterCompensation);
	m_openedDocumentActions.addAction(actionRightCutterCompensation);
	m_openedDocumentActions.addAction(actionResetCutterCompensation);
	m_openedDocumentActions.addAction(actionPocketSelection);
	m_openedDocumentActions.addAction(actionHideSelection);
	m_openedDocumentActions.addAction(actionShowHidden);
	m_openedDocumentActions.addAction(actionTransformSelection);
	m_openedDocumentActions.addAction(actionMirrorSelection);
	m_openedDocumentActions.addAction(actionSetSelectionOrigin);
	m_openedDocumentActions.addAction(actionSimulate);

	m_openedDocumentActions.setExclusive(true);
}

void MainWindow::setDocumentToolsEnabled(bool enabled)
{
	m_openedDocumentActions.setEnabled(enabled);
}

QString MainWindow::defaultFileName(const QString &extension) const
{
	return m_app.lastHandledFileBaseName() + extension;
}


MainWindow::MainWindow(model::Application &app)
	:m_app(app),
	m_openedDocumentActions(this)
{
	setupUi();
	setupMenuActions();
	setupOpenedDocumentActions();
	showMaximized();

	setDocumentToolsEnabled(false);

	connect(&m_app, &model::Application::titleChanged, this, &MainWindow::setWindowTitle);
	connect(&m_app, &model::Application::documentChanged, this, &MainWindow::documentChanged);
	connect(&m_app, &model::Application::errorRaised, this, &MainWindow::displayError);
}

void MainWindow::openFile()
{
	const QString fileName = QFileDialog::getOpenFileName(this);
	if (!fileName.isEmpty() && !m_app.loadFile(fileName)) {
		QMessageBox::critical(this, "Error", "Invalid file type " + fileName);
	}
}

void MainWindow::saveFile()
{
	const QString lastFileName = m_app.lastSavedDxfplotFileName();
	if (lastFileName.isEmpty()) {
		saveAsFile();
	}
	else if (const QString fileName = defaultFileName(model::Application::FileExtension::Dxfplot); !m_app.saveToDxfplot(fileName)) {
		QMessageBox::critical(this, "Error", "Couldn't save " + fileName);
	}
}

void MainWindow::saveAsFile()
{
	const QString defaultPath = defaultFileName(model::Application::FileExtension::Dxfplot);
	const QString fileName = QFileDialog::getSaveFileName(this, "Save As File", defaultPath, "Text files (*.dxfplot)");

	if (!fileName.isEmpty() && !m_app.saveToDxfplot(fileName)) {
		QMessageBox::critical(this, "Error", "Couldn't save " + fileName);
	}
}

void MainWindow::exportFile()
{
	const QString lastFileName = m_app.lastSavedGcodeFileName();
	if (lastFileName.isEmpty()) {
		exportAsFile();
	}
	else if (const QString fileName = defaultFileName(model::Application::FileExtension::Gcode); !m_app.saveToGcode(fileName)) {
		QMessageBox::critical(this, "Error", "Couldn't save " + fileName);
	}
}

void MainWindow::exportAsFile()
{
	const QString defaultPath = defaultFileName(model::Application::FileExtension::Gcode);
	const QString fileName = QFileDialog::getSaveFileName(this, "Export File", defaultPath, "Text files (*.ngc *.txt)");

	if (!fileName.isEmpty() && !m_app.saveToGcode(fileName)) {
		QMessageBox::critical(this, "Error", "Couldn't save " + fileName);
	}
}

void MainWindow::openSettings()
{
	config::Config newConfig = m_app.config();

	const bool accepted = [&newConfig]{
		settings::Settings settings(newConfig);
		return (settings.exec() == QDialog::Accepted);
	}();

	if (accepted) {
		m_app.setConfig(std::move(newConfig));
	}
}

void MainWindow::transformSelection()
{
	dialogs::Transform transform;
	if (transform.exec() == QDialog::Accepted) {
		m_app.transformSelection(transform.matrix());
	}
}

void MainWindow::mirrorSelection()
{
	dialogs::Mirror mirror;
	if (mirror.exec() == QDialog::Accepted) {
		m_app.transformSelection(mirror.matrix());
	}
}

void MainWindow::setSelectionOrigin()
{
	const geometry::Rect selectionBoundingRect = m_app.selectionBoundingRect();

	dialogs::SetOrigin setOrigin(selectionBoundingRect);
	if (setOrigin.exec() == QDialog::Accepted) {
		m_app.transformSelection(setOrigin.matrix()); // TODO common function
	}
}

void MainWindow::documentChanged(model::Document *newDocument)
{
	setDocumentToolsEnabled((newDocument != nullptr));

	m_simulation->hide();
}

void MainWindow::displayError(const QString &message)
{
	QMessageBox messageBox;
	messageBox.critical(this, "Error", message);
}

void MainWindow::simulate()
{
	model::Simulation simulation = m_app.createSimulation();
	m_simulation->setSimulation(std::move(simulation));
	m_simulation->show();
}

}

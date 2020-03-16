#include <mainwindow.h>
#include <path.h>
#include <task.h>
#include <viewport.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QSplitter>
#include <QDebug>

namespace View
{

QWidget *MainWindow::setupLeftPanel()
{
	Task *task = new Task(m_app.task());
	Path *path = new Path(m_app.task());

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

MainWindow::MainWindow(Control::Application &app)
	:m_app(app)
{
	setupUi();

	showMaximized();

	connect(actionOpen, &QAction::triggered, this, &MainWindow::openFile);
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

}

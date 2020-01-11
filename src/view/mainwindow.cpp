#include <mainwindow.h>
#include <task.h>
#include <viewport.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QSplitter>
#include <QDebug>

namespace View
{

MainWindow::MainWindow(Control::Application &app)
	:m_app(app),
	m_task(new Task(m_app)),
	m_viewport(new Viewport(m_app)) // TODO
{
	setupUi(this);

	QSplitter *vertSplitter = new QSplitter(Qt::Vertical, this);
	vertSplitter->addWidget(m_task);

	QSplitter *horiSplitter = new QSplitter(Qt::Horizontal, this);
	horiSplitter->addWidget(vertSplitter);
	horiSplitter->addWidget(m_viewport);
	horiSplitter->setStretchFactor(0, 0);
	horiSplitter->setStretchFactor(1, 1);

	horizontalLayout->addWidget(horiSplitter);

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

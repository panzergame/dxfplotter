#include <mainwindow.h>

#include <importer/dxf/importer.h>

#include <core/assembler.h>

#include <QFileDialog>
#include <QMimeDatabase>
#include <QMessageBox>

#include <QDebug>

namespace View
{

MainWindow::MainWindow(const QString &fileName)
{
	Ui::MainWindow::setupUi(this);
	showMaximized();

	connect(actionOpen, &QAction::triggered, this, &MainWindow::openFile);

	if (!fileName.isEmpty()) {
		if (!loadFile(fileName)) {
			qCritical() << "Invalid file type " + fileName;
		}
	}
}

bool MainWindow::loadFile(const QString &fileName)
{
	const QMimeDatabase db;
	const QMimeType mime = db.mimeTypeForFile(fileName);

	if (mime.name() == "image/vnd.dxf") {
		loadDxf(fileName);
	}
	else if (mime.name() == "text/plain") {
		loadPlot(fileName);
	}
	else {
		return false;
	}

	return true;
}

void MainWindow::loadDxf(const QString &fileName)
{
	Importer::Dxf::Importer imp(qPrintable(fileName));
	Core::Assembler assembler(imp.polylines(), 0.001); // TODO
	const Core::Polylines polylines = assembler.mergedPolylines();
	for (const Core::Polyline &polyline : polylines) {
		std::cout << polyline << std::endl;
	}
}

void MainWindow::loadPlot(const QString &fileName)
{
	
}

void MainWindow::openFile()
{
	const QString fileName = QFileDialog::getOpenFileName(this);
	if (!fileName.isEmpty()) {
		if (!loadFile(fileName)) {
			QMessageBox messageBox;
			messageBox.critical(this, "Error", "Invalid file type " + fileName);
		}
	}
}

}

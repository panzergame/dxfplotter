#include <application.h>
#include <geometry/assembler.h>

#include <importer/dxf/importer.h>

#include <QMimeDatabase>
#include <QDebug>

namespace Control
{

Application::Application(const QString &fileName)
{
	if (!fileName.isEmpty()) {
		if (!loadFile(fileName)) {
			qCritical() << "Invalid file type " + fileName;
		}
	}
}

Model::Task &Application::task()
{
	return m_task;
}

Model::Path::List &Application::paths()
{
	return m_paths;
}

bool Application::loadFile(const QString &fileName)
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

void Application::loadDxf(const QString &fileName)
{
	// Import data
	Importer::Dxf::Importer imp(qPrintable(fileName));
	// Merge polylines to create longest contours
	Geometry::Assembler assembler(imp.polylines(), 0.001); // TODO tolerance
	Geometry::Polyline::List polylines = assembler.mergedPolylines();

	/*for (const Geometry::Polyline &polyline : polylines) { // TODO debug
		std::cout << polyline << std::endl;
	}*/

	m_paths = Model::PathsFromPolylines(std::move(polylines));
	m_task = Model::Task(m_paths);
}

void Application::loadPlot(const QString &fileName)
{
	
}

}

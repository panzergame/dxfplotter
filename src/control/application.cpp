#include <application.h>
#include <geometry/assembler.h>

#include <importer/dxf/importer.h>
#include <exporter/gcode/exporter.h>

#include <common/exception.h>

#include <QMimeDatabase>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

namespace Control
{

static const QString configFileName = "config.ini";

/** Retrieves application config file path
 * @return config file path
 */
static std::string configFilePath()
{
	const QDir dir = QDir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));

	// Ensure the path exists
	dir.mkpath(".");

	const QString path = dir.filePath(configFileName);

	return path.toStdString();
}

Application::Application(const QString &fileName)
	:m_config(configFilePath())
{
	if (!fileName.isEmpty()) {
		if (!loadFile(fileName)) {
			qCritical() << "Invalid file type " + fileName;
		}
	}
}

Model::Task *Application::task()
{
	return m_task;
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

bool Application::loadDxf(const QString &fileName)
{
	Geometry::Polyline::List polylines;
	try {
		// Import data
		Importer::Dxf::Importer imp(fileName.toStdString());
		polylines = imp.polylines();
	}
	catch (const Common::FileException &e) {
		return false;
	}

	// Merge polylines to create longest contours
	Geometry::Assembler assembler(std::move(polylines), m_config.dxf().assembleTolerance);
	Geometry::Polyline::List mergedPolylines = assembler.mergedPolylines();

	const Model::PathSettings defaultPathSettings(120.0f, 200.0f); // TODO config extract

	m_paths = Model::Path::FromPolylines(std::move(mergedPolylines), defaultPathSettings);
	m_task = new Model::Task(this, m_paths);

	return true;
}

void Application::loadPlot(const QString &fileName)
{
	
}

bool Application::exportToGcode(const QString &fileName)
{
	// Copy gcode format from config file
	Exporter::GCode::Format format(m_config.gcodeFormat());

	try {
		Exporter::GCode::Exporter exporter(m_task, format, fileName.toStdString());
	}
	catch (const Common::FileException &e) {
		return false;
	}

	return true;
}

}

#include <application.h>
#include <geometry/assembler.h>
#include <geometry/cleaner.h>

#include <importer/dxf/importer.h>
#include <exporter/gcode/exporter.h>

#include <common/exception.h>

#include <QMimeDatabase>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

namespace Model
{

static const QString configFileName = "config.yml";

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

PathSettings Application::defaultPathSettings() const
{
	const Config::Section &defaultPath = m_importConfig.section("default_path");
	return PathSettings(defaultPath.var("feed_rate"), defaultPath.var("intensity"), defaultPath.var("passes"));
}

void Application::cutterCompensation(float scale)
{
	const Config::Section &dxf = m_importConfig.section("dxf");

	const float radius = m_toolConfig->section("general").var("radius");
	const float scaledRadius = radius * scale;

	m_task->forEachSelectedPath([scaledRadius, minimumPolylineLength=(float)dxf.var("minimum_polyline_length"),
		minimumArcLength=(float)dxf.var("minimum_arc_length")](Model::Path *path){
			path->offset(scaledRadius, minimumPolylineLength, minimumArcLength);
	});
}

Application::Application()
	:m_config(Config::Config(configFilePath())),
	m_importConfig(m_config.root().group("import")),
	// Default select first tool
	m_toolConfig(&m_config.root().group("tools").group(0))
{
}

Config::Config &Application::config()
{
	return m_config;
}

bool Application::selectTool(const QString &toolName)
{
	const Config::Group &tools = m_config.root().group("tools");
	const std::string name = toolName.toStdString();
	const bool exists = tools.has(name);

	if (exists) {
		m_toolConfig = &tools.group(name);
	}

	return exists;
}

void Application::selectToolFromCmd(const QString &toolName)
{
	if (!selectTool(toolName)) {
		qCritical() << "Invalid tool name " << toolName;
	}
}

void Application::loadFileFromCmd(const QString &fileName)
{
	if (!fileName.isEmpty()) {
		if (!loadFile(fileName)) {
			qCritical() << "Invalid file type " << fileName;
		}
	}
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

	// Update window title based on file name.
	const QString title = QFileInfo(fileName).fileName();
	emit titleChanged(title);

	return true;
}

bool Application::loadDxf(const QString &fileName)
{
	const Config::Section &dxf = m_importConfig.section("dxf");

	Geometry::Polyline::List polylines;
	try {
		// Import data
		Importer::Dxf::Importer imp(fileName.toStdString(), dxf.var("spline_to_arc_precision"), dxf.var("minimum_spline_length"));
		polylines = imp.polylines();
	}
	catch (const Common::FileException &e) {
		return false;
	}

	// Merge polylines to create longest contours
	Geometry::Assembler assembler(std::move(polylines), dxf.var("assemble_tolerance"));
	// Remove small bulges
	Geometry::Cleaner cleaner(assembler.polylines(), dxf.var("minimum_polyline_length"), dxf.var("minimum_arc_length"));

	m_paths = Path::FromPolylines(cleaner.polylines(), defaultPathSettings());
	m_task = new Task(this, m_paths);

	emit taskChanged(m_task);

	return true;
}

void Application::loadPlot(const QString &fileName)
{
	
}

bool Application::exportToGcode(const QString &fileName)
{
	const Config::Section& gcode = m_toolConfig->section("gcode");
	// Copy gcode format from config file
	Exporter::GCode::Format format(gcode);

	try {
		Exporter::GCode::Exporter exporter(m_task, format, fileName.toStdString());
	}
	catch (const Common::FileException &e) {
		return false;
	}

	return true;
}

void Application::leftCutterCompensation()
{
	cutterCompensation(-1.0f);
}

void Application::rightCutterCompensation()
{
	cutterCompensation(1.0f);
}

void Application::resetCutterCompensation()
{
	m_task->forEachSelectedPath([](Model::Path *path){ path->resetOffset(); });
}

}

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

void Application::selectToolConfig(const Config::Tools::Tool &tool)
{
	m_selectedToolConfig = &tool;
	emit selectedToolConfigChanged(tool);
}

PathSettings Application::defaultPathSettings() const
{
	const Config::Import::DefaultPath &defaultPath = m_importConfig.defaultPath();
	return PathSettings(defaultPath.planeFeedRate(), defaultPath.depthFeedRate(), defaultPath.intensity(), defaultPath.depth());
}

void Application::cutterCompensation(float scale)
{
	const Config::Import::Dxf &dxf = m_importConfig.dxf();

	const float radius = m_selectedToolConfig->general().radius();
	const float scaledRadius = radius * scale;

	m_task->forEachSelectedPath([scaledRadius, minimumPolylineLength=(float)dxf.minimumPolylineLength(),
		minimumArcLength=(float)dxf.minimumArcLength()](Model::Path *path){
			path->offset(scaledRadius, minimumPolylineLength, minimumArcLength);
	});
}

Application::Application()
	:m_config(Config::Config(configFilePath())),
	m_importConfig(m_config.root().import())
{
	// Default select first tool
	selectToolConfig(m_config.root().tools().first());
}

Config::Config &Application::config()
{
	return m_config;
}

void Application::setConfig(Config::Config &&config)
{
	m_config = std::move(config);
	emit configChanged(m_config);
}

bool Application::selectTool(const QString &toolName)
{
	const Config::Tools &tools = m_config.root().tools();
	const std::string name = toolName.toStdString();
	const bool exists = tools.has(name);

	if (exists) {
		selectToolConfig(tools[name]);
	}

	return exists;
}

void Application::selectToolFromCmd(const QString &toolName)
{
	if (!selectTool(toolName)) {
		qCritical() << "Invalid tool name " << toolName;
	}
}

QString Application::currentFileBaseName() const
{
	return m_currentFileBaseName;
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
	const QFileInfo fileInfo(fileName);
	const QString title = fileInfo.fileName();
	emit titleChanged(title);

	m_currentFileBaseName = fileInfo.absoluteDir().filePath(fileInfo.baseName());

	return true;
}

bool Application::loadDxf(const QString &fileName)
{
	const Config::Import::Dxf &dxf = m_importConfig.dxf();

	Geometry::Polyline::List polylines;
	try {
		// Import data
		Importer::Dxf::Importer imp(fileName.toStdString(), dxf.splineToArcPrecision(), dxf.minimumSplineLength());
		polylines = imp.polylines();
	}
	catch (const Common::FileException &e) {
		return false;
	}

	// Merge polylines to create longest contours
	Geometry::Assembler assembler(std::move(polylines), dxf.assembleTolerance());
	// Remove small bulges
	Geometry::Cleaner cleaner(assembler.polylines(), dxf.minimumPolylineLength(), dxf.minimumArcLength());

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
	try {
		Exporter::GCode::Exporter exporter(m_task, *m_selectedToolConfig, fileName.toStdString());
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

void Application::hideSelection()
{
	m_task->forEachSelectedPath([](Model::Path *path){
		path->setVisible(false);
	});
}

void Application::showHidden()
{
	m_task->forEachPath([](Model::Path *path){
		if (!path->visible()) {
			path->setVisible(true);
			path->setSelected(true);
		}
	});
}

}

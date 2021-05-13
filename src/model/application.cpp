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

void Application::selectProfileConfig(const Config::Profiles::Profile &profile)
{
	m_selectedProfileConfig = &profile;
	emit selectedProfileConfigChanged(profile);
}

PathSettings Application::defaultPathSettings() const
{
	const Config::Profiles::Profile::DefaultPath &defaultPath = m_selectedProfileConfig->defaultPath();
	return PathSettings(defaultPath.planeFeedRate(), defaultPath.depthFeedRate(), defaultPath.intensity(), defaultPath.depth());
}

void Application::cutterCompensation(float scale)
{
	const Config::Import::Dxf &dxf = m_config.root().import().dxf();

	const float radius = m_selectedToolConfig->general().radius();
	const float scaledRadius = radius * scale;

	m_task->forEachSelectedPath([scaledRadius, minimumPolylineLength=(float)dxf.minimumPolylineLength(),
		minimumArcLength=(float)dxf.minimumArcLength()](Model::Path &path){
			path.offset(scaledRadius, minimumPolylineLength, minimumArcLength);
	});
}

Application::Application()
	:m_config(Config::Config(configFilePath()))
{
	// Default select first tool
	selectToolConfig(m_config.root().tools().first());

	// Default select first profile
	selectProfileConfig(m_config.root().profiles().first());
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

bool Application::selectProfile(const QString &profileName)
{
	const Config::Profiles &profiles = m_config.root().profiles();
	const std::string name = profileName.toStdString();
	const bool exists = profiles.has(name);

	if (exists) {
		selectProfileConfig(profiles[name]);
	}

	return exists;
}

void Application::selectProfileFromCmd(const QString &profileName)
{
	if (!selectProfile(profileName)) {
		qCritical() << "Invalid profile name " << profileName;
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
	const Config::Import::Dxf &dxf = m_config.root().import().dxf();

	Importer::Dxf::Layer::List importerLayers;
	try {
		// Import data by layers
		Importer::Dxf::Importer imp(fileName.toStdString(), dxf.splineToArcPrecision(), dxf.minimumSplineLength());
		importerLayers = imp.layers();
	}
	catch (const Common::FileException &e) {
		return false;
	}

	Path::ListUPtr paths;
	Layer::ListUPtr layers;
	for (Importer::Dxf::Layer &importerLayer : importerLayers) {
		// Merge polylines to create longest contours
		Geometry::Assembler assembler(importerLayer.polylines(), dxf.assembleTolerance());
		// Remove small bulges
		Geometry::Cleaner cleaner(assembler.polylines(), dxf.minimumPolylineLength(), dxf.minimumArcLength());

		const std::string &layerName = importerLayer.name();

		Layer *layer = new Layer(layerName);
		layers.emplace_back(layer);

		// Create paths from merged and cleaned polylines of one layer
		Path::ListUPtr children = Path::FromPolylines(cleaner.polylines(), defaultPathSettings(), *layer);
		layer->setChildren(children);

		paths.insert(paths.end(), std::make_move_iterator(children.begin()), std::make_move_iterator(children.end()));
	}

	m_task.reset(new Task(std::move(paths), std::move(layers)));

	emit taskChanged(m_task.get());

	return true;
}

void Application::loadPlot(const QString &fileName)
{
	
}

bool Application::exportToGcode(const QString &fileName)
{
	try {
		Exporter::GCode::Exporter exporter(*m_task, *m_selectedToolConfig, m_selectedProfileConfig->gcode(), fileName.toStdString());
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
	m_task->forEachSelectedPath([](Model::Path &path){ path.resetOffset(); });
}

void Application::hideSelection()
{
	m_task->forEachSelectedPath([](Model::Path &path){
		path.setVisible(false);
	});
}

void Application::showHidden()
{
	m_task->forEachPath([](Model::Path &path){
		if (!path.visible()) {
			path.setVisible(true);
			path.setSelected(true);
		}
	});
}

}

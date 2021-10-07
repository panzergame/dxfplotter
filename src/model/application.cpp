#include <application.h>
#include <geometry/assembler.h>
#include <geometry/cleaner.h>

#include <importer/dxf/importer.h>
#include <importer/dxfplot/importer.h>

#include <exporter/gcode/exporter.h>
#include <exporter/dxfplot/exporter.h>

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
	const Config::Profiles::Profile::DefaultPath &defaultPath = m_defaultProfileConfig->defaultPath();
	return PathSettings(defaultPath.planeFeedRate(), defaultPath.depthFeedRate(), defaultPath.intensity(), defaultPath.depth());
}

std::optional<const Config::Tools::Tool *> Application::findTool(const std::string &name) const
{
	const Config::Tools &tools = m_config.root().tools();
	if (tools.has(name)) {
		return std::make_optional(&tools[name]);
	}

	return std::nullopt;
}

std::optional<const Config::Profiles::Profile *> Application::findProfile(const std::string &name) const
{
	const Config::Profiles &profiles = m_config.root().profiles();
	if (profiles.has(name)) {
		return std::make_optional(&profiles[name]);
	}

	return std::nullopt;
}

void Application::cutterCompensation(float scale)
{
	const Config::Import::Dxf &dxf = m_config.root().import().dxf();

	const float radius = m_openedDocument->toolConfig().general().radius();
	const float scaledRadius = radius * scale;

	m_openedDocument->task().forEachSelectedPath([scaledRadius, minimumPolylineLength=(float)dxf.minimumPolylineLength(),
		minimumArcLength=(float)dxf.minimumArcLength()](Model::Path &path){
			path.offset(scaledRadius, minimumPolylineLength, minimumArcLength);
	});
}

Task::UPtr Application::createTaskFromDxfImporter(const Importer::Dxf::Importer& importer)
{
	const Config::Import::Dxf &dxf = m_config.root().import().dxf();

	Layer::ListUPtr layers;
	for (Importer::Dxf::Layer &importerLayer : importer.layers()) {
		// Merge polylines to create longest contours
		Geometry::Assembler assembler(importerLayer.polylines(), dxf.assembleTolerance());
		// Remove small bulges
		Geometry::Cleaner cleaner(assembler.polylines(), dxf.minimumPolylineLength(), dxf.minimumArcLength());

		const std::string &layerName = importerLayer.name();

		// Create paths from merged and cleaned polylines of one layer
		Path::ListUPtr children = Path::FromPolylines(cleaner.polylines(), defaultPathSettings(), layerName);

		Layer::UPtr& layer = layers.emplace_back(std::make_unique<Layer>(layerName, std::move(children)));
	}

	return std::make_unique<Task>(std::move(layers));
}

Application::Application()
	:m_config(configFilePath()),
	// Default select first tool
	m_defaultToolConfig(&m_config.root().tools().first()),
	// Default select first profile
	m_defaultProfileConfig(&m_config.root().profiles().first())
{
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
	const std::string name = toolName.toStdString();
	const std::optional<const Config::Tools::Tool *> tool = findTool(name);

	if (tool) {
		if (m_openedDocument) {
			m_openedDocument->setToolConfig(**tool);
		}
		m_defaultToolConfig = *tool;

		return true;
	}

	return false;
}

void Application::defaultToolFromCmd(const QString &toolName)
{
	if (!selectTool(toolName)) {
		qCritical() << "Invalid tool name " << toolName;
	}
}

bool Application::selectProfile(const QString &profileName)
{
	const std::string name = profileName.toStdString();
	const std::optional<const Config::Profiles::Profile *> profile = findProfile(name);

	if (profile) {
		if (m_openedDocument) {
			m_openedDocument->setProfileConfig(**profile);
		}
		m_defaultProfileConfig = *profile;

		return true;
	}

	return false;
}

void Application::defaultProfileFromCmd(const QString &profileName)
{
	if (!selectProfile(profileName)) {
		qCritical() << "Invalid profile name " << profileName;
	}
}

const QString &Application::currentImportedFileBaseName() const
{
	return m_currentImportedFileBaseName;
}

const QString &Application::currentDxfplotFileName() const
{
	return m_currentDxfplotFileName;
}

void Application::loadFileFromCmd(const QString &fileName)
{
	if (!fileName.isEmpty()) {
		loadFile(fileName);
	}
}

bool Application::loadFile(const QString &fileName)
{
	qInfo() << "Opening " << fileName;

	m_currentDxfplotFileName = "";
	m_currentImportedFileBaseName = "";

	const QMimeDatabase db;
	const QMimeType mime = db.mimeTypeForFile(fileName);
	const QString mineName = mime.name();

	if (mineName == "image/vnd.dxf") {
		if (!loadFromDxf(fileName)) {
			return false;
		}
	}
	else if (mineName == "text/plain") {
		loadFromDxfplot(fileName);
	}
	else {
		qCritical() << "Invalid file type: " << fileName;
		return false;
	}

	// Update window title based on file name.
	const QFileInfo fileInfo(fileName);
	const QString title = fileInfo.fileName();
	emit titleChanged(title);

	return true;
}

bool Application::loadFromDxf(const QString &fileName)
{
	const Config::Import::Dxf &dxf = m_config.root().import().dxf();

	try {
		Importer::Dxf::Importer importer(fileName.toStdString(), dxf.splineToArcPrecision(), dxf.minimumSplineLength());
 
		m_openedDocument = std::make_unique<Document>(createTaskFromDxfImporter(importer), *m_defaultToolConfig, *m_defaultProfileConfig);
	}
	catch (const Common::FileCouldNotOpenException&) {
		qCritical() << "File not found:" << fileName;
		return false;
	}

	const QFileInfo fileInfo(fileName);
	m_currentImportedFileBaseName = fileInfo.absoluteDir().filePath(fileInfo.baseName());

	emit documentChanged(m_openedDocument.get());

	return true;
}

bool Application::loadFromDxfplot(const QString &fileName)
{
	try {
		Importer::Dxfplot::Importer importer(m_config.root().tools(), m_config.root().profiles());
 
		m_openedDocument = importer(fileName.toStdString());
	}
	catch (const Common::FileCouldNotOpenException&) {
		return false;
	}

	m_currentDxfplotFileName = fileName;

	emit documentChanged(m_openedDocument.get());

	return true;
}

bool Application::saveToGcode(const QString &fileName) const
{
	std::ofstream file(fileName.toStdString());
	if (file) {
		try {
			Exporter::GCode::Exporter exporter(m_openedDocument->toolConfig(), m_openedDocument->profileConfig().gcode());
			return saveToFile(exporter, fileName);
		}
		catch (const std::exception &exception) {
			emit errorRaised(exception.what());
		}
	}

	return false;
}

bool Application::saveToDxfplot(const QString &fileName)
{
	Exporter::Dxfplot::Exporter exporter;
	
	const bool succeedSaving = saveToFile(exporter, fileName);
	if (succeedSaving) {
		m_currentDxfplotFileName = fileName;
	}

	return succeedSaving;
}

void Application::leftCutterCompensation()
{
	cutterCompensation(1.0f);
}

void Application::rightCutterCompensation()
{
	cutterCompensation(-1.0f);
}

void Application::resetCutterCompensation()
{
	Task &task = m_openedDocument->task();
	task.forEachSelectedPath([](Model::Path &path){ path.resetOffset(); });
}

void Application::hideSelection()
{
	Task &task = m_openedDocument->task();
	task.forEachSelectedPath([](Model::Path &path){
		path.setVisible(false);
	});
}

void Application::showHidden()
{
	Task &task = m_openedDocument->task();
	task.forEachPath([](Model::Path &path){
		if (!path.visible()) {
			path.setVisible(true);
			path.setSelected(true);
		}
	});
}

}

module;

#include <fstream>
import importer.dxf.importer;
import importer.dxfplot.importer;
import exporter.gcode.exporter;
import exporter.dxfplot.exporter;
#include <QObject>
#include <QDebug>
#include <QMimeDatabase>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QtCore/qtmochelpers.h>

export module model.application;

import common.exception;
import config.config;
import geometry.filter.assembler;
import geometry.filter.cleaner;
import geometry.filter.removeexactduplicate;
import model.document;
import model.documenthistory;
import model.simulation;
import geometry.polyline;
import geometry.rect;
import importer.dxf.layer;
import model.path;
import model.pathsettings;
import model.task;
namespace model
{

static const QString configFileName = "config.yml";

static std::string configFilePath()
{
	const QDir dir = QDir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));

	// Ensure the path exists
	dir.mkpath(".");

	const QString path = dir.filePath(configFileName);

	return path.toStdString();
}

}

export namespace model
{

class Application : public QObject
{
	Q_OBJECT;

private:
	/// Global configuration
	config::Config m_config;

	const config::Tools::Tool* m_defaultToolConfig;
	const config::Profiles::Profile* m_defaultProfileConfig;

	// Last opened or saved file base name.
	QString m_lastHandledFileBaseName;
	QString m_lastSavedGcodeFileName;
	QString m_lastSavedDxfplotFileName;

	Document::UPtr m_openedDocument;
	DocumentHistory::UPtr m_documentHistory;

	void setOpenedDocument(Document::UPtr&& document)
	{
		m_openedDocument = std::move(document);
		m_documentHistory = std::make_unique<DocumentHistory>(*m_openedDocument);

		emit newDocumentOpened(m_openedDocument.get());
	}

	void setRestoredDocument(const Document& documentVersion)
	{
		m_openedDocument = std::make_unique<Document>(documentVersion);
		emit documentRestoredFromHistory(m_openedDocument.get());
	}

	static QString baseName(const QString& fileName)
	{
		const QFileInfo fileInfo(fileName);
		return fileInfo.absoluteDir().filePath(fileInfo.baseName());
	}

	void resetLastSavedFileNames()
	{
		m_lastSavedDxfplotFileName.clear();
		m_lastSavedGcodeFileName.clear();
	}

	PathSettings defaultPathSettings() const
	{
		const config::Profiles::Profile::DefaultPath& defaultPath = m_defaultProfileConfig->defaultPath();
		return PathSettings(defaultPath.planeFeedRate(), defaultPath.depthFeedRate(), defaultPath.intensity(),
							defaultPath.depth());
	}

	const config::Tools::Tool* findTool(const std::string& name) const
	{
		const config::Tools& tools = m_config.root().tools();
		if (tools.has(name)) {
			return &tools[name];
		}

		return nullptr;
	}

	const config::Profiles::Profile* findProfile(const std::string& name) const
	{
		const config::Profiles& profiles = m_config.root().profiles();
		if (profiles.has(name)) {
			return &profiles[name];
		}

		return nullptr;
	}

	void cutterCompensation(float scale)
	{
		const config::Import::Dxf& dxf = m_config.root().import().dxf();

		const float radius = m_openedDocument->toolConfig().general().radius();
		const float scaledRadius = radius * scale;

		Task& task = m_openedDocument->task();
		task.cutterCompensationSelection(scaledRadius, dxf.minimumPolylineLength(), dxf.minimumArcLength());
	}

	geometry::Polyline::List postProcessImportedPolylines(geometry::Polyline::List&& rawPolylines) const
	{
		const config::Import::Dxf& dxf = m_config.root().import().dxf();

		geometry::filter::RemoveExactDuplicate removeExactDuplicate(std::move(rawPolylines));

		// Merge polylines to create longest contours
		geometry::filter::Assembler assembler(removeExactDuplicate.polylines(), dxf.assembleTolerance());
		// Remove small bulges
		geometry::filter::Cleaner cleaner(assembler.polylines(), dxf.minimumPolylineLength(), dxf.minimumArcLength());

		return cleaner.polylines();
	}

	Task::UPtr createTaskFromDxfImporter(const importer::dxf::Importer& importer)
	{
		Layer::ListUPtr layers;
		for (importer::dxf::Layer& importerLayer : importer.layers()) {
			const std::string& layerName = importerLayer.name();
			geometry::Polyline::List polylines = postProcessImportedPolylines(importerLayer.polylines());

			// Create paths from merged and cleaned polylines of one layer
			Path::ListUPtr children = Path::FromPolylines(std::move(polylines), defaultPathSettings(), layerName);

			layers.emplace_back(std::make_unique<Layer>(layerName, std::move(children)));
		}

		Task::UPtr task = std::make_unique<Task>(std::move(layers));

		const config::Import::Dxf& dxf = m_config.root().import().dxf();
		if (dxf.sortPathByLength()) {
			task->sortPathsByLength();
		}

		return task;
	}

	template<class Exporter>
	bool saveToFile(Exporter&& exporter, const QString& fileName)
	{
		qInfo() << "Saving to " << fileName;
		std::ofstream output(fileName.toStdString());
		if (output) {
			exporter(*m_openedDocument, output);
			m_lastHandledFileBaseName = baseName(fileName);
			emit fileSaved(fileName);
			return true;
		}

		return false;
	}

public:
	struct FileExtension
	{
		inline static const QString Gcode = ".ngc";
		inline static const QString Dxfplot = ".dxfplot";
	};

	explicit Application()
		: m_config(configFilePath())
		,
		// Default select first tool
		m_defaultToolConfig(&m_config.root().tools().first())
		,
		// Default select first profile
		m_defaultProfileConfig(&m_config.root().profiles().first())
	{
	}

	config::Config& config() { return m_config; }

	void setConfig(config::Config&& config)
	{
		m_config = std::move(config);
		emit configChanged(m_config);
	}

	/// Select tool used as configuration for further operations
	bool selectTool(const QString& toolName)
	{
		const std::string name = toolName.toStdString();
		const config::Tools::Tool* tool = findTool(name);

		if (tool) {
			if (m_openedDocument) {
				m_openedDocument->setToolConfig(*tool);
				emit toolChanged();
			}
			m_defaultToolConfig = tool;

			return true;
		}

		return false;
	}

	void defaultToolFromCmd(const QString& toolName)
	{
		if (!selectTool(toolName)) {
			qCritical() << "Invalid tool name " << toolName;
		}
	}

	/// Select profile used as configuration for further operations
	bool selectProfile(const QString& profileName)
	{
		const std::string name = profileName.toStdString();
		const config::Profiles::Profile* profile = findProfile(name);

		if (profile) {
			if (m_openedDocument) {
				m_openedDocument->setProfileConfig(*profile);
			}
			m_defaultProfileConfig = profile;

			return true;
		}

		return false;
	}

	void defaultProfileFromCmd(const QString& profileName)
	{
		if (!selectProfile(profileName)) {
			qCritical() << "Invalid profile name " << profileName;
		}
	}

	const QString& lastHandledFileBaseName() const { return m_lastHandledFileBaseName; }

	const QString& lastSavedDxfplotFileName() const { return m_lastSavedDxfplotFileName; }

	const QString& lastSavedGcodeFileName() const { return m_lastSavedGcodeFileName; }

	void loadFileFromCmd(const QString& fileName)
	{
		if (!fileName.isEmpty()) {
			loadFile(fileName);
		}
	}

	bool loadFile(const QString& fileName)
	{
		qInfo() << "Opening " << fileName;

		const QMimeDatabase db;
		const QMimeType mime = db.mimeTypeForFile(fileName);
		const QString mineName = mime.name();

		if (mineName == "image/vnd.dxf") {
			if (!loadFromDxf(fileName)) {
				return false;
			}
		} else if (mineName == "text/plain") {
			loadFromDxfplot(fileName);
		} else {
			qCritical() << "Invalid file type: " << fileName;
			return false;
		}

		m_lastHandledFileBaseName = baseName(fileName);
		resetLastSavedFileNames();

		// Update window title based on file name.
		const QFileInfo fileInfo(fileName);
		const QString title = fileInfo.fileName();
		emit titleChanged(title);

		return true;
	}

	bool loadFromDxf(const QString& fileName)
	{
		const config::Import::Dxf& dxf = m_config.root().import().dxf();

		try {
			importer::dxf::Importer importer(fileName.toStdString(), dxf.splineToArcPrecision(),
											 dxf.minimumSplineLength(), dxf.minimumArcLength());

			setOpenedDocument(std::make_unique<Document>(createTaskFromDxfImporter(importer), *m_defaultToolConfig,
														 *m_defaultProfileConfig));
		} catch (const common::FileCouldNotOpenException&) {
			qCritical() << "File not found:" << fileName;
			return false;
		}

		return true;
	}

	bool loadFromDxfplot(const QString& fileName)
	{
		try {
			importer::dxfplot::Importer importer(m_config.root().tools(), m_config.root().profiles());

			setOpenedDocument(importer(fileName.toStdString()));
		} catch (const common::FileCouldNotOpenException&) {
			return false;
		}

		return true;
	}

	bool saveToGcode(const QString& fileName)
	{
		try {
			const exporter::gcode::Exporter::Options options = static_cast<exporter::gcode::Exporter::Options>(
				exporter::gcode::Exporter::ExportConfig | exporter::gcode::Exporter::ExportMetadata);

			exporter::gcode::Exporter exporter(m_openedDocument->toolConfig(), m_openedDocument->profileConfig(),
											   options);
			const bool saved = saveToFile(exporter, fileName);
			if (saved) {
				m_lastSavedGcodeFileName = fileName;
			}
			return saved;
		} catch (const std::exception& exception) {
			emit errorRaised(exception.what());
		}

		return false;
	}

	bool saveToDxfplot(const QString& fileName)
	{
		exporter::dxfplot::Exporter exporter;

		const bool saved = saveToFile(exporter, fileName);
		if (saved) {
			m_lastSavedDxfplotFileName = fileName;
		}

		return saved;
	}

	void leftCutterCompensation()
	{
		cutterCompensation(1.0f);

		takeDocumentSnapshot();
	}

	void rightCutterCompensation()
	{
		cutterCompensation(-1.0f);

		takeDocumentSnapshot();
	}

	void resetCutterCompensation()
	{
		Task& task = m_openedDocument->task();
		task.resetCutterCompensationSelection();

		takeDocumentSnapshot();
	}

	void pocketSelection()
	{
		const config::Import::Dxf& dxf = m_config.root().import().dxf();
		const float radius = m_openedDocument->toolConfig().general().radius();

		Task& task = m_openedDocument->task();
		task.pocketSelection(radius, dxf.minimumPolylineLength(), dxf.minimumArcLength());

		takeDocumentSnapshot();
	}

	geometry::Rect selectionBoundingRect() const
	{
		Task& task = m_openedDocument->task();
		return task.selectionBoundingRect();
	}

	void transformSelection(const QTransform& matrix)
	{
		Task& task = m_openedDocument->task();
		task.transformSelection(matrix);

		takeDocumentSnapshot();
	}

	void hideSelection()
	{
		Task& task = m_openedDocument->task();
		task.hideSelection();

		takeDocumentSnapshot();
	}

	void showHidden()
	{
		Task& task = m_openedDocument->task();
		task.showHidden();

		takeDocumentSnapshot();
	}

	void optimizeOrder()
	{
		const config::Optimize& optimize = m_config.root().optimize();

		Task& task = m_openedDocument->task();
		task.optimizeOrder(optimize.maintainPathLengthOrder(), optimize.lengthPrecision(),
						   optimize.distancePrecision());
	}

	Simulation createSimulation()
	{
		const float fastMoveFeedRate = m_config.root().simulation().fastMoveFeedRate();
		return Simulation(*m_openedDocument, fastMoveFeedRate);
	}

	void takeDocumentSnapshot() { m_documentHistory->takeSnapshot(*m_openedDocument); }

	void undoDocumentChanges() { setRestoredDocument(m_documentHistory->undo()); }

	void redoDocumentChanges() { setRestoredDocument(m_documentHistory->redo()); }

Q_SIGNALS:
	void newDocumentOpened(Document* newDocument);
	void documentRestoredFromHistory(Document* newDocument);
	void titleChanged(QString title);
	void configChanged(config::Config& config);
	void toolChanged();
	void errorRaised(const QString& message) const;
	void fileSaved(const QString& fileName);
};

}

#include "application.moc"

#pragma once

#include <model/document.h>
#include <model/documenthistory.h>
#include <model/simulation.h>
#include <config/config.h>

#include <QObject>
#include <QDebug>

#include <fstream>

namespace importer::dxf
{
	class Importer;
}

namespace model
{


class Application : public QObject
{
	Q_OBJECT;

private:
	/// Global configuration
	config::Config m_config;

	const config::Tools::Tool *m_defaultToolConfig;
	const config::Profiles::Profile *m_defaultProfileConfig;

	// Last opened or saved file base name.
	QString m_lastHandledFileBaseName;
	QString m_lastSavedGcodeFileName;
	QString m_lastSavedDxfplotFileName;

	Document::UPtr m_openedDocument;
	DocumentHistory::UPtr m_documentHistory;

	void setOpenedDocument(Document::UPtr &&document);
	void setRestoredDocument(const Document &documentVersion);

	static QString baseName(const QString& fileName);	
	void resetLastSavedFileNames();

	PathSettings defaultPathSettings() const;

	const config::Tools::Tool *findTool(const std::string &name) const;
	const config::Profiles::Profile *findProfile(const std::string &name) const;

	void cutterCompensation(float scale);

	geometry::Polyline::List postProcessImportedPolylines(geometry::Polyline::List &&rawPolylines) const;
	Task::UPtr createTaskFromDxfImporter(const importer::dxf::Importer& importer);

	template <class Exporter>
	bool saveToFile(Exporter &&exporter, const QString &fileName)
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

	template <class Exporter>
	QByteArray saveToBuffer(Exporter &&exporter)
	{
		std::stringstream outputStream;
		exporter(*m_openedDocument, outputStream);
		const std::string output = outputStream.str();

		return QByteArray(output.data(), output.size());
	}

	enum class SupportFileType {
		Dxf,
		Dxfplot
	};

	std::optional<SupportFileType> findFileType(const QString &fileName) const;

public:
	struct FileExtension {
		inline static const QString Gcode = ".ngc";
		inline static const QString Dxfplot = ".dxfplot";
	};

	explicit Application();

	config::Config &config();
	void setConfig(config::Config &&config);

	/// Select tool used as configuration for further operations
	bool selectTool(const QString &toolName);
	void defaultToolFromCmd(const QString &toolName);

	/// Select profile used as configuration for further operations
	bool selectProfile(const QString &profileName);
	void defaultProfileFromCmd(const QString &profileName);

	const QString &lastHandledFileBaseName() const;
	const QString &lastSavedDxfplotFileName() const;
	const QString &lastSavedGcodeFileName() const;
	void loadFileFromCmd(const QString &fileName);
	bool loadFile(const QString &fileName);
	bool loadFile(const QString &fileName, const QByteArray &fileContent);
	bool loadFile(const QString &fileName, SupportFileType fileType, const QByteArray &fileContent);
	bool loadFromDxf(std::istream &fileContent);
	bool loadFromDxfplot(const QString &fileName);

	bool saveToGcode(const QString &fileName);
	QByteArray saveToGcode();
	bool saveToDxfplot(const QString &fileName);
	QByteArray saveToDxfplot();

	void leftCutterCompensation();
	void rightCutterCompensation();
	void resetCutterCompensation();
	void pocketSelection();

	geometry::Rect selectionBoundingRect() const;
	void transformSelection(const QTransform& matrix);

	void hideSelection();
	void showHidden();

	Simulation createSimulation();

	void takeDocumentSnapshot();
	void undoDocumentChanges();
	void redoDocumentChanges();

Q_SIGNALS:
	void newDocumentOpened(Document *newDocument);
	void documentRestoredFromHistory(Document *newDocument);
	void titleChanged(QString title);
	void configChanged(config::Config &config);
	void errorRaised(const QString& message) const;
	void fileSaved(const QString &fileName);
};

}

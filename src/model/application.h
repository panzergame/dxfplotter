#pragma once

#include <model/document.h>
#include <config/config.h>

#include <QObject>
#include <QDebug>

#include <fstream>

namespace Importer::Dxf
{
	class Importer;
}

namespace Model
{

class Application : public QObject
{
	Q_OBJECT;

private:
	/// Global configuration
	Config::Config m_config;

	const Config::Tools::Tool *m_defaultToolConfig;
	const Config::Profiles::Profile *m_defaultProfileConfig;

	// Absolute file basename of current imported file
	QString m_currentImportedFileBaseName;
	QString m_currentDxfplotFileName;

	Document::UPtr m_openedDocument;

	PathSettings defaultPathSettings() const;

	const Config::Tools::Tool *findTool(const std::string &name) const;
	const Config::Profiles::Profile *findProfile(const std::string &name) const;

	void cutterCompensation(float scale);

	Task::UPtr createTaskFromDxfImporter(const Importer::Dxf::Importer& importer);

	template <class Exporter>
	bool saveToFile(Exporter &exporter, const QString &fileName) const
	{
		qInfo() << "Saving to " << fileName;
		std::ofstream output(fileName.toStdString());
		if (output) {
			exporter(*m_openedDocument, output);
			return true;
		}

		return false;
	}

public:
	explicit Application();

	Config::Config &config();
	void setConfig(Config::Config &&config);

	/// Select tool used as configuration for further operations
	bool selectTool(const QString &toolName);
	void defaultToolFromCmd(const QString &toolName);

	/// Select profile used as configuration for further operations
	bool selectProfile(const QString &profileName);
	void defaultProfileFromCmd(const QString &profileName);

	const QString &currentImportedFileBaseName() const;
	const QString &currentDxfplotFileName() const;
	void loadFileFromCmd(const QString &fileName);
	bool loadFile(const QString &fileName);
	bool loadFromDxf(const QString &fileName);
	bool loadFromDxfplot(const QString &fileName);

	bool saveToGcode(const QString &fileName) const;
	bool saveToDxfplot(const QString &fileName);

	void leftCutterCompensation();
	void rightCutterCompensation();
	void resetCutterCompensation();

	void transformSelection(const QTransform& matrix);

	void hideSelection();
	void showHidden();

Q_SIGNALS:
	void documentChanged(Document *newDocument);
	void titleChanged(QString title);
	void configChanged(Config::Config &config);
	void errorRaised(const QString& message) const;
};

}

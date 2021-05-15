#pragma once

#include <model/task.h>
#include <model/layer.h>
#include <config/config.h>

#include <importer/dxf/importer.h>

#include <QObject>

namespace Model
{

class Application : public QObject
{
	Q_OBJECT;

private:
	/// Global configuration
	Config::Config m_config;
	/// Selected tool configuration
	const Config::Tools::Tool *m_selectedToolConfig;
	/// Selected profile configuration
	const Config::Profiles::Profile *m_selectedProfileConfig;

	// Absolute file basename of current loaded file
	QString m_currentFileBaseName;

	Task::UPtr m_task;

	PathSettings defaultPathSettings() const;

	void selectToolConfig(const Config::Tools::Tool &tool);
	void selectProfileConfig(const Config::Profiles::Profile &profile);

	void cutterCompensation(float scale);

	Task::UPtr createTaskFromDxfImporter(const Importer::Dxf::Importer& importer);

public:
	explicit Application();

	Config::Config &config();
	void setConfig(Config::Config &&config);

	/// Select tool used as configuration for further operations
	bool selectTool(const QString &toolName);
	void selectToolFromCmd(const QString &toolName);

	/// Select profile used as configuration for further operations
	bool selectProfile(const QString &profileName);
	void selectProfileFromCmd(const QString &profileName);

	QString currentFileBaseName() const;
	void loadFileFromCmd(const QString &fileName);
	bool loadFile(const QString &fileName);
	bool loadDxf(const QString &fileName);
	void loadPlot(const QString &fileName);

	bool exportToGcode(const QString &fileName);

	void leftCutterCompensation();
	void rightCutterCompensation();
	void resetCutterCompensation();

	void hideSelection();
	void showHidden();

Q_SIGNALS:
	void taskChanged(Task *newTask);
	void titleChanged(QString title);
	void selectedToolConfigChanged(const Config::Tools::Tool &tool);
	void selectedProfileConfigChanged(const Config::Profiles::Profile &profile);
	void configChanged(Config::Config &config);
	void errorRaised(const QString& message);
};

}

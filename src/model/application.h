#pragma once

#include <model/task.h>
#include <config/config.h>

#include <QObject>

namespace Model
{

class Application : public QObject
{
	Q_OBJECT;

private:
	/// Global configuration
	Config::Config m_config;
	/// Importing configuration (e.g dxf, default path values)
	const Config::Import &m_importConfig;
	/// Selected tool configuration
	const Config::Tools::Tool *m_toolConfig;

	Path::ListPtr m_paths; // TODO parent destruct
	Task *m_task;

	PathSettings defaultPathSettings() const;

	void cutterCompensation(float scale);

public:
	explicit Application();

	Config::Config &config();
	void setConfig(Config::Config &&config);

	/// Select tool used as configuration for further operations
	bool selectTool(const QString &toolName);
	void selectToolFromCmd(const QString &toolName);

	void loadFileFromCmd(const QString &fileName);
	bool loadFile(const QString &fileName);
	bool loadDxf(const QString &fileName);
	void loadPlot(const QString &fileName);

	bool exportToGcode(const QString &fileName);

	void leftCutterCompensation();
	void rightCutterCompensation();
	void resetCutterCompensation();

Q_SIGNALS:
	void taskChanged(Task *newTask);
	void titleChanged(QString title);
	void configChanged(Config::Config &config);
};

}

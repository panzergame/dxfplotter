#pragma once

#include <model/task.h>
#include <model/config.h>

#include <QObject>

namespace Model
{

class Application : public QObject
{
	Q_OBJECT;

private:
	Config m_config;

	Path::ListPtr m_paths; // TODO parent destruct
	Task *m_task;

	QString m_currentFileName;

public:
	explicit Application(const QString &fileName);

	Model::Task *task();

	bool loadFile(const QString &fileName);
	bool loadDxf(const QString &fileName);
	void loadPlot(const QString &fileName);
	bool exportToGcode(const QString &fileName);
};

}

#pragma once

#include <model/task.h>
#include <model/config.h>

#include <QObject>

namespace Control // TODO move in model because it's near to Task/Path goal'
{

class Application : public QObject
{
	Q_OBJECT;

private:
	Model::Config m_config;

	Model::Path::ListPtr m_paths; // TODO parent destruct
	Model::Task *m_task;

	QString m_currentFileName;

public:
	explicit Application(const QString &fileName);

	Model::Task *task();

	bool loadFile(const QString &fileName);
	void loadDxf(const QString &fileName);
	void loadPlot(const QString &fileName);
	void exportToGcode(const QString &fileName);
};

}

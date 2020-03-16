#pragma once

#include <model/task.h>

#

#include <QObject>

namespace Control
{

class Application : public QObject
{
	Q_OBJECT;

private:
	Model::Path::ListPtr m_paths; // TODO parent destruct
	Model::Task *m_task;

	QString m_currentFileName;

public:
	explicit Application(const QString &fileName);

	Model::Task *task();

	bool loadFile(const QString &fileName);
	void loadDxf(const QString &fileName);
	void loadPlot(const QString &fileName);
};

}

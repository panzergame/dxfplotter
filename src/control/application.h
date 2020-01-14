#pragma once

#include <model/task.h>

#include <QObject>

namespace Control
{

class Application : public QObject
{
private:
	Model::Paths m_paths;
	Model::Task m_task;

	QString m_currentFileName;

public:
	explicit Application(const QString &fileName);

	Model::Task &task();
	Model::Paths &paths();

	bool loadFile(const QString &fileName);
	void loadDxf(const QString &fileName);
	void loadPlot(const QString &fileName);
};

}

#pragma once

#include <ui_task.h>

#include <model/task.h>

#include <QWidget>

namespace View
{

class Task : public QWidget, private Ui::Task
{
private:
	Model::Task *m_task;

	void setupModel();

public:
	explicit Task(Model::Task *task);

protected Q_SLOTS:
	void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
};

}

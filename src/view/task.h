#pragma once

#include <uic/ui_task.h>

#include <model/task.h>

#include <QWidget>
#include <QItemSelectionModel>
#include <model/taskmodelobserver.h>

namespace View
{

class TaskListModel;

class Task : public Model::TaskModelObserver<QWidget>, private Ui::Task
{
private:
	std::unique_ptr<TaskListModel> m_model;
	bool m_outsideSelectionBlocked;

	void setupModel();

	void changeItemSelection(Model::Path *path, QItemSelectionModel::SelectionFlag flag);

public:
	explicit Task(Model::Application &app);

protected:
	void taskChanged();

protected Q_SLOTS:
	void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
	void pathSelected(Model::Path *path);
	void pathDeselected(Model::Path *path);
};

}

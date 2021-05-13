#pragma once

#include <uic/ui_task.h>

#include <model/task.h>

#include <QWidget>
#include <QItemSelectionModel>
#include <model/taskmodelobserver.h>

namespace View::Task
{

class TaskListModel;

class Task : public Model::TaskModelObserver<QWidget>, private Ui::Task
{
private:
	std::unique_ptr<TaskListModel> m_model;
	bool m_outsideSelectionBlocked;

	void setupModel();
	void setupController();

	void changeItemSelection(const Model::Path &path, QItemSelectionModel::SelectionFlag flag);

public:
	explicit Task(Model::Application &app);

protected:
	void taskChanged();

protected Q_SLOTS:
	void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
	void pathSelectedChanged(Model::Path &path, bool selected);
	void moveCurrentPath(Model::Task::MoveDirection direction);
};

}

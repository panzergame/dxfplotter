#include <task.h>

#include <tasklistmodel.h>

#include <QDebug>

namespace View
{

Task::Task(Control::Application &app)
	:m_app(app)
{
	setupUi(this);

	setupModel();
}

void Task::setupModel()
{
	TaskListModel *model = new TaskListModel(m_app.task(), this);
	treeView->setModel(model);

	// Configure selection model
	QItemSelectionModel *selectionModel = treeView->selectionModel();
	connect(selectionModel, &QItemSelectionModel::selectionChanged, this, &Task::selectionChanged);
}

void Task::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	const Model::Task &task = m_app.task();

	for (const QModelIndex &index : selected.indexes()) {
		Model::Path *path = task.pathAt(index.row());
		path->select();
	}

	for (const QModelIndex &index : deselected.indexes()) {
		Model::Path *path = task.pathAt(index.row());
		path->deselect();
	}
}

}

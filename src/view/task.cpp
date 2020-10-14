#include <task.h>

#include <tasklistmodel.h>

#include <QDebug>

namespace View
{

Task::Task(Model::Application &app)
	:TaskModelObserver(app),
	m_outsideSelectionBlocked(false)
{
	setupUi(this);
}

void Task::setupModel()
{
	m_model.reset(new TaskListModel(m_task, this)),
	treeView->setModel(m_model.get());

	// Configure selection model
	QItemSelectionModel *selectionModel = treeView->selectionModel();
	selectionModel->
	connect(selectionModel, &QItemSelectionModel::selectionChanged, this, &Task::selectionChanged);

	// Track outside path selection, e.g from graphics view.
	connect(m_task, &Model::Task::pathSelected, this, &Task::pathSelected);
	connect(m_task, &Model::Task::pathDeselected, this, &Task::pathDeselected);

	connect(moveUp, &QPushButton::pressed, [this](){ moveCurrentPath(Model::Task::MoveDirection::UP); });
	connect(moveDown, &QPushButton::pressed, [this](){ moveCurrentPath(Model::Task::MoveDirection::DOWN); });
}

void Task::changeItemSelection(Model::Path *path, QItemSelectionModel::SelectionFlag flag)
{
	// Break signal loop
	if (!m_outsideSelectionBlocked) {
		QItemSelectionModel *selectionModel = treeView->selectionModel();
		const int index = m_task->indexFor(path);
		selectionModel->select(m_model->index(index, 0), flag);
	}
}

void Task::taskChanged()
{
	setupModel();
}

void Task::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	// Disallows feedback from path selection
	m_outsideSelectionBlocked = true;

	for (const QModelIndex &index : selected.indexes()) {
		Model::Path *path = m_task->pathAt(index.row());
		path->select();
	}

	for (const QModelIndex &index : deselected.indexes()) {
		Model::Path *path = m_task->pathAt(index.row());
		path->deselect();
	}

	m_outsideSelectionBlocked = false;
}

void Task::pathSelected(Model::Path *path)
{
	changeItemSelection(path, QItemSelectionModel::Select);
}

void Task::pathDeselected(Model::Path *path)
{
	changeItemSelection(path, QItemSelectionModel::Deselect);
}

void Task::moveCurrentPath(Model::Task::MoveDirection direction)
{
	QItemSelectionModel *selectionModel = treeView->selectionModel();
	const QModelIndex selectedIndex = m_model->movePath(selectionModel->currentIndex(), direction);
	selectionModel->setCurrentIndex(selectedIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Current);
}

}

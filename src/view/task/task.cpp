#include <task.h>

#include <tasklistmodel.h>

#include <QLabel>
#include <QDebug>

namespace View::Task
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
	pathsTreeView->setModel(m_model.get());

	QHeaderView *header = pathsTreeView->header();
	header->setStretchLastSection(false);
	header->setSectionResizeMode(0, QHeaderView::Stretch);
}

void Task::setupController()
{
	// Synchronize selection in 2D view
	QItemSelectionModel *selectionModel = pathsTreeView->selectionModel();
	connect(selectionModel, &QItemSelectionModel::selectionChanged, this, &Task::selectionChanged);

	connect(pathsTreeView, &QTreeView::clicked, m_model.get(), &TaskListModel::itemClicked);

	// Track outside path selection, e.g from graphics view.
	connect(m_task, &Model::Task::pathSelectedChanged, this, &Task::pathSelectedChanged);

	connect(moveUp, &QPushButton::pressed, [this](){ moveCurrentPath(Model::Task::MoveDirection::UP); });
	connect(moveDown, &QPushButton::pressed, [this](){ moveCurrentPath(Model::Task::MoveDirection::DOWN); });
}

void Task::changeItemSelection(const Model::Path &path, QItemSelectionModel::SelectionFlag flag)
{
	// Break signal loop
	if (!m_outsideSelectionBlocked) {
		QItemSelectionModel *selectionModel = pathsTreeView->selectionModel();
		const int index = m_task->indexFor(path);
		selectionModel->select(m_model->index(index, 0), flag);
	}
}

void Task::taskChanged()
{
	setupModel();
	setupController();
}

void Task::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	// Disallows feedback from path selection
	m_outsideSelectionBlocked = true;

	for (const QModelIndex &index : selected.indexes()) {
		Model::Path &path = m_task->pathAt(index.row());
		path.setSelected(true);
	}

	for (const QModelIndex &index : deselected.indexes()) {
		Model::Path &path = m_task->pathAt(index.row());
		path.setSelected(false);
	}

	m_outsideSelectionBlocked = false;
}

void Task::pathSelectedChanged(Model::Path &path, bool selected)
{
	changeItemSelection(path,
			selected ? QItemSelectionModel::Select : QItemSelectionModel::Deselect);
}

void Task::moveCurrentPath(Model::Task::MoveDirection direction)
{
	QItemSelectionModel *selectionModel = pathsTreeView->selectionModel();
	const QModelIndex currentSelectedIndex = selectionModel->currentIndex();
	const QModelIndex newSelectedIndex = m_model->movePath(currentSelectedIndex, direction);
	selectionModel->setCurrentIndex(newSelectedIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Current);
}

}

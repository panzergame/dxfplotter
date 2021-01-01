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
	treeView->setModel(m_model.get());
}

void Task::setupController()
{
	// Synchronize selection in 2D view
	QItemSelectionModel *selectionModel = treeView->selectionModel();
	connect(selectionModel, &QItemSelectionModel::selectionChanged, this, &Task::selectionChanged);

	connect(m_model.get(), &QAbstractItemModel::rowsMoved,
		[](const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row)
	{
		
	});

	// Track outside path selection, e.g from graphics view.
	connect(m_task, &Model::Task::pathSelectedChanged, this, &Task::pathSelectedChanged);

	connect(moveUp, &QPushButton::pressed, [this](){ moveCurrentPath(Model::Task::MoveDirection::UP); });
	connect(moveDown, &QPushButton::pressed, [this](){ moveCurrentPath(Model::Task::MoveDirection::DOWN); });
}

void Task::setupAllPathEditor()
{
	for (int row = 0, count = m_model->rowCount(); row < count; ++row) {
		setupPathEditor(m_model->index(row, 1));
	}
}

void Task::setupPathEditor(const QModelIndex &index)
{
	QWidget *cell = new QLabel(QString::number(index.row()));
	treeView->setIndexWidget(index, cell);
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
	setupController();
// 	setupAllPathEditor();
}

void Task::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	// Disallows feedback from path selection
	m_outsideSelectionBlocked = true;

	for (const QModelIndex &index : selected.indexes()) {
		Model::Path *path = m_task->pathAt(index.row());
		path->setSelected(true);
	}

	for (const QModelIndex &index : deselected.indexes()) {
		Model::Path *path = m_task->pathAt(index.row());
		path->setSelected(false);
	}

	m_outsideSelectionBlocked = false;
}

void Task::pathSelectedChanged(Model::Path *path, bool selected)
{
	changeItemSelection(path,
			selected ? QItemSelectionModel::Select : QItemSelectionModel::Deselect);
}

void Task::moveCurrentPath(Model::Task::MoveDirection direction)
{
	QItemSelectionModel *selectionModel = treeView->selectionModel();
	const QModelIndex currentSelectedIndex = selectionModel->currentIndex();
	const QModelIndex newSelectedIndex = m_model->movePath(currentSelectedIndex, direction);
	selectionModel->setCurrentIndex(newSelectedIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Current);
}

}

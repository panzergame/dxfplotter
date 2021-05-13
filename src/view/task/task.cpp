#include <task.h>

#include <pathlistmodel.h>
#include <layertreemodel.h>

#include <QLabel>
#include <QDebug>

namespace View::Task
{

Task::Task(Model::Application &app)
	:TaskModelObserver(app)
{
	setupUi(this);
}

void Task::setupModel()
{
	m_pathListModel.reset(setupTreeViewModel<PathListModel>(pathsTreeView));
	m_layerTreeModel.reset(setupTreeViewModel<LayerTreeModel>(layersTreeView));

	layersTreeView->expandAll();
}

void Task::setupController()
{
	// Track outside path selection, e.g from graphics view.
	connect(m_task, &Model::Task::pathSelectedChanged, this, &Task::pathSelectedChanged);

	setupTreeViewController(m_pathListModel, pathsTreeView);
	setupTreeViewController(m_layerTreeModel, layersTreeView);

	connect(moveUp, &QPushButton::pressed, [this](){ moveCurrentPath(Model::Task::MoveDirection::UP); });
	connect(moveDown, &QPushButton::pressed, [this](){ moveCurrentPath(Model::Task::MoveDirection::DOWN); });
}

void Task::updateItemSelection(const Model::Path &path, QItemSelectionModel::SelectionFlag flag)
{
	m_pathListModel->updateItemSelection(path, flag, pathsTreeView->selectionModel());
	m_layerTreeModel->updateItemSelection(path, flag, layersTreeView->selectionModel());
}

void Task::taskChanged()
{
	setupModel();
	setupController();
}

void Task::pathSelectedChanged(Model::Path &path, bool selected)
{
	updateItemSelection(path,
			selected ? QItemSelectionModel::Select : QItemSelectionModel::Deselect);
}

void Task::moveCurrentPath(Model::Task::MoveDirection direction)
{
	QItemSelectionModel *selectionModel = pathsTreeView->selectionModel();
	const QModelIndex currentSelectedIndex = selectionModel->currentIndex();
	const QModelIndex newSelectedIndex = m_pathListModel->movePath(currentSelectedIndex, direction);
	selectionModel->setCurrentIndex(newSelectedIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Current);
}

}

#include <task.h>

#include <pathlistmodel.h>
#include <layertreemodel.h>

#include <QLabel>
#include <QDebug>

namespace view::task
{

Task::Task(model::Application &app)
	:DocumentModelObserver(app),
	m_app(app)
{
	setupUi(this);
}

void Task::setupModel()
{
	m_pathListModel = setupTreeViewModel<PathListModel>(pathsTreeView);
	m_layerTreeModel = setupTreeViewModel<LayerTreeModel>(layersTreeView);

	layersTreeView->expandAll();
}

void Task::setupController()
{
	// Track outside path selection, e.g from graphics view.
	connect(&task(), &model::Task::pathSelectedChanged, this, &Task::pathSelectedChanged);

	setupTreeViewController(m_pathListModel, pathsTreeView);
	setupTreeViewController(m_layerTreeModel, layersTreeView);

	connect(moveUp, &QPushButton::pressed, [this](){ moveCurrentPath(model::Task::MoveDirection::UP); });
	connect(moveDown, &QPushButton::pressed, [this](){ moveCurrentPath(model::Task::MoveDirection::DOWN); });
	connect(moveTop, &QPushButton::pressed, [this](){ moveCurrentPathToTip(model::Task::MoveTip::Top); });
	connect(moveBottom, &QPushButton::pressed, [this](){ moveCurrentPathToTip(model::Task::MoveTip::Bottom); });
}

void Task::updateItemSelection(const model::Path &path, QItemSelectionModel::SelectionFlag flag)
{
	m_pathListModel->updateItemSelection(path, flag, pathsTreeView->selectionModel());
	m_layerTreeModel->updateItemSelection(path, flag, layersTreeView->selectionModel());
}

void Task::documentChanged()
{
	setupModel();
	setupController();
}

void Task::pathSelectedChanged(model::Path &path, bool selected)
{
	updateItemSelection(path,
			selected ? QItemSelectionModel::Select : QItemSelectionModel::Deselect);
}

void Task::moveCurrentPath(model::Task::MoveDirection direction)
{
	QItemSelectionModel *selectionModel = pathsTreeView->selectionModel();
	const QModelIndex currentSelectedIndex = selectionModel->currentIndex();
	const QModelIndex newSelectedIndex = m_pathListModel->movePath(currentSelectedIndex, direction);
	selectionModel->setCurrentIndex(newSelectedIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Current);

	m_app.takeDocumentSnapshot();
}

void Task::documentVisibilityChanged()
{
	m_app.takeDocumentSnapshot();
}

void Task::moveCurrentPathToTip(model::Task::MoveTip tip)
{
	QItemSelectionModel *selectionModel = pathsTreeView->selectionModel();
	const QModelIndex currentSelectedIndex = selectionModel->currentIndex();
	const QModelIndex newSelectedIndex = m_pathListModel->movePathToTip(currentSelectedIndex, tip);
	selectionModel->setCurrentIndex(newSelectedIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Current);
}

}

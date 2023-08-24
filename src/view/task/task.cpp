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

	connect(moveUp, &QPushButton::pressed, [this](){ moveCurrentPathToDirection(model::Task::MoveDirection::UP); });
	connect(moveDown, &QPushButton::pressed, [this](){ moveCurrentPathToDirection(model::Task::MoveDirection::DOWN); });
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

void Task::moveCurrentPathToDirection(model::Task::MoveDirection direction)
{
	moveCurrentPath([this, direction](const QModelIndex& index){
		m_pathListModel->movePathToDirection(index, direction);
	});
}

void Task::documentVisibilityChanged()
{
	m_app.takeDocumentSnapshot();
}

void Task::moveCurrentPathToTip(model::Task::MoveTip tip)
{
	moveCurrentPath([this, tip](const QModelIndex& index){
		m_pathListModel->movePathToTip(index, tip);
	});
}

void Task::rebuildSelectionFromTask()
{
	QItemSelectionModel *pathsTreeSelectionModel = pathsTreeView->selectionModel();
	QItemSelectionModel *layersTreeSelectionModel = layersTreeView->selectionModel();

	m_pathListModel->clearSelection(pathsTreeSelectionModel);
	m_layerTreeModel->clearSelection(layersTreeSelectionModel);

	task().forEachSelectedPath([this, pathsTreeSelectionModel, layersTreeSelectionModel](const model::Path& path){
		constexpr QItemSelectionModel::SelectionFlag flag = QItemSelectionModel::Select;

		m_pathListModel->updateItemSelection(path, flag, pathsTreeSelectionModel);
		m_layerTreeModel->updateItemSelection(path, flag, layersTreeSelectionModel);
	});
}

}

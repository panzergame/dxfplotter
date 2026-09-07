module;

#include <uic/ui_task.h>
#include <QWidget>
#include <QItemSelectionModel>
#include <QLabel>
#include <QDebug>

export module view.task.task;
import view.task.layertreemodel;
import view.task.pathlistmodel;

import model.documentmodelobserver;
import model.task;
import model.application;
import model.path;

export namespace view::task
{

class Task : public model::DocumentModelObserver<QWidget>, private Ui::Task
{
private:
	model::Application& m_app;

	std::unique_ptr<PathListModel> m_pathListModel;
	std::unique_ptr<LayerTreeModel> m_layerTreeModel;

	template<class Model>
	std::unique_ptr<Model> setupTreeViewModel(QTreeView* treeView)
	{
		std::unique_ptr<Model> model = std::make_unique<Model>(task(), this);
		treeView->setModel(model.get());

		QHeaderView* header = treeView->header();
		header->setStretchLastSection(false);
		header->setSectionResizeMode(0, QHeaderView::Stretch);
		header->setSectionResizeMode(1, QHeaderView::ResizeToContents);

		return model;
	}

	template<class Model>
	void setupTreeViewController(std::unique_ptr<Model>& model, QTreeView* treeView)
	{
		// Synchronize selection in 2D view
		QItemSelectionModel* selectionModel = treeView->selectionModel();
		connect(selectionModel, &QItemSelectionModel::selectionChanged, model.get(), &Model::selectionChanged);

		connect(treeView, &QTreeView::clicked, model.get(), &Model::itemClicked);

		connect(model.get(), &Model::documentVisibilityChanged, this, &Task::documentVisibilityChanged);
	}

	void setupModel();
	void setupController();

	void updateItemSelection(const model::Path& path, QItemSelectionModel::SelectionFlag flag);

	void moveCurrentPathToDirection(model::Task::MoveDirection direction);
	void moveCurrentPathToTip(model::Task::MoveTip tip);

	template<class Func>
	void moveCurrentPath(Func&& movement)
	{
		QItemSelectionModel* selectionModel = pathsTreeView->selectionModel();

		const QModelIndexList selectedItems = selectionModel->selectedIndexes();
		for (const QModelIndex& selectedIndex : selectedItems) {
			movement(selectedIndex);
		}

		rebuildSelectionFromTask();

		m_app.takeDocumentSnapshot();
	}

	void rebuildSelectionFromTask();

public:
	explicit Task(model::Application& app);

protected:
	void documentChanged();

protected Q_SLOTS:
	void pathSelectedChanged(model::Path& path, bool selected);
	void documentVisibilityChanged();
};

}

namespace view::task
{

Task::Task(model::Application& app)
	: DocumentModelObserver(app)
	, m_app(app)
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

	connect(moveUp, &QPushButton::pressed, [this]() {
		moveCurrentPathToDirection(model::Task::MoveDirection::UP);
	});
	connect(moveDown, &QPushButton::pressed, [this]() {
		moveCurrentPathToDirection(model::Task::MoveDirection::DOWN);
	});
	connect(moveTop, &QPushButton::pressed, [this]() {
		moveCurrentPathToTip(model::Task::MoveTip::Top);
	});
	connect(moveBottom, &QPushButton::pressed, [this]() {
		moveCurrentPathToTip(model::Task::MoveTip::Bottom);
	});
}

void Task::updateItemSelection(const model::Path& path, QItemSelectionModel::SelectionFlag flag)
{
	m_pathListModel->updateItemSelection(path, flag, pathsTreeView->selectionModel());
	m_layerTreeModel->updateItemSelection(path, flag, layersTreeView->selectionModel());
}

void Task::documentChanged()
{
	setupModel();
	setupController();
}

void Task::pathSelectedChanged(model::Path& path, bool selected)
{
	updateItemSelection(path, selected ? QItemSelectionModel::Select : QItemSelectionModel::Deselect);
}

void Task::moveCurrentPathToDirection(model::Task::MoveDirection direction)
{
	moveCurrentPath([this, direction](const QModelIndex& index) {
		m_pathListModel->movePathToDirection(index, direction);
	});
}

void Task::documentVisibilityChanged()
{
	m_app.takeDocumentSnapshot();
}

void Task::moveCurrentPathToTip(model::Task::MoveTip tip)
{
	moveCurrentPath([this, tip](const QModelIndex& index) {
		m_pathListModel->movePathToTip(index, tip);
	});
}

void Task::rebuildSelectionFromTask()
{
	QItemSelectionModel* pathsTreeSelectionModel = pathsTreeView->selectionModel();
	QItemSelectionModel* layersTreeSelectionModel = layersTreeView->selectionModel();

	m_pathListModel->clearSelection(pathsTreeSelectionModel);
	m_layerTreeModel->clearSelection(layersTreeSelectionModel);

	task().forEachSelectedPath([this, pathsTreeSelectionModel, layersTreeSelectionModel](const model::Path& path) {
		constexpr QItemSelectionModel::SelectionFlag flag = QItemSelectionModel::Select;

		m_pathListModel->updateItemSelection(path, flag, pathsTreeSelectionModel);
		m_layerTreeModel->updateItemSelection(path, flag, layersTreeSelectionModel);
	});
}

}

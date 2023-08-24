#pragma once

#include <uic/ui_task.h>

#include <model/task.h>

#include <QWidget>
#include <QItemSelectionModel>
#include <model/documentmodelobserver.h>

namespace view::task
{

class PathListModel;
class LayerTreeModel;

class Task : public model::DocumentModelObserver<QWidget>, private Ui::Task
{
private:
	model::Application &m_app;

	std::unique_ptr<PathListModel> m_pathListModel;
	std::unique_ptr<LayerTreeModel> m_layerTreeModel;

	template <class Model>
	std::unique_ptr<Model> setupTreeViewModel(QTreeView *treeView)
	{
		std::unique_ptr<Model> model = std::make_unique<Model>(task(), this);
		treeView->setModel(model.get());

		QHeaderView *header = treeView->header();
		header->setStretchLastSection(false);
		header->setSectionResizeMode(0, QHeaderView::Stretch);
		header->setSectionResizeMode(1, QHeaderView::ResizeToContents);

		return model;
	}

	template <class Model>
	void setupTreeViewController(std::unique_ptr<Model>& model, QTreeView *treeView)
	{
		// Synchronize selection in 2D view
		QItemSelectionModel *selectionModel = treeView->selectionModel();
		connect(selectionModel, &QItemSelectionModel::selectionChanged, model.get(), &Model::selectionChanged);

		connect(treeView, &QTreeView::clicked, model.get(), &Model::itemClicked);

		connect(model.get(), &Model::documentVisibilityChanged, this, &Task::documentVisibilityChanged);
	}

	void setupModel();
	void setupController();

	void updateItemSelection(const model::Path &path, QItemSelectionModel::SelectionFlag flag);

	void moveCurrentPathToDirection(model::Task::MoveDirection direction);
	void moveCurrentPathToTip(model::Task::MoveTip tip);

	template <class Func>
	void moveCurrentPath(Func &&movement)
	{
		QItemSelectionModel *selectionModel = pathsTreeView->selectionModel();

		const QModelIndexList selectedItems = selectionModel->selectedIndexes();
		for (const QModelIndex& selectedIndex : selectedItems) {
			movement(selectedIndex);
		}

		rebuildSelectionFromTask();

		m_app.takeDocumentSnapshot();
	}

	void rebuildSelectionFromTask();

public:
	explicit Task(model::Application &app);

protected:
	void documentChanged();

protected Q_SLOTS:
	void pathSelectedChanged(model::Path &path, bool selected);
	void documentVisibilityChanged();
};

}

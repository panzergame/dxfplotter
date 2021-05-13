#pragma once

#include <uic/ui_task.h>

#include <model/task.h>

#include <QWidget>
#include <QItemSelectionModel>
#include <model/taskmodelobserver.h>

namespace View::Task
{

class PathListModel;
class LayerTreeModel;

class Task : public Model::TaskModelObserver<QWidget>, private Ui::Task
{
private:
	std::unique_ptr<PathListModel> m_pathListModel;
	std::unique_ptr<LayerTreeModel> m_layerTreeModel;

	template <class Model>
	Model *setupTreeViewModel(QTreeView *treeView)
	{
		Model *model = new Model(*m_task, this);
		treeView->setModel(model);

		QHeaderView *header = treeView->header();
		header->setStretchLastSection(false);
		header->setSectionResizeMode(0, QHeaderView::Stretch);

		return model;
	}

	template <class Model>
	void setupTreeViewController(std::unique_ptr<Model>& model, QTreeView *treeView)
	{
		// Synchronize selection in 2D view
		QItemSelectionModel *selectionModel = treeView->selectionModel();
		connect(selectionModel, &QItemSelectionModel::selectionChanged, model.get(), &Model::selectionChanged);

		connect(treeView, &QTreeView::clicked, model.get(), &Model::itemClicked);
	}

	void setupModel();
	void setupController();

	void updateItemSelection(const Model::Path &path, QItemSelectionModel::SelectionFlag flag);

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

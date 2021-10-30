#pragma once

#include <cassert>

#include <model/task.h>

#include <QAbstractItemModel>
#include <QItemSelectionModel>

namespace view::task
{

class LayerTreeModel: public QAbstractItemModel
{
    Q_OBJECT

private:
	model::Task &m_task;

public:
	explicit LayerTreeModel(model::Task &task, QObject *parent);

	QVariant data(const QModelIndex &index, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;

	void itemClicked(const QModelIndex &index);
	void updateItemSelection(const model::Path &path, QItemSelectionModel::SelectionFlag flag, QItemSelectionModel *selectionModel);
	void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
};

}

#pragma once

#include <cassert>

#include <model/task.h>

#include <QAbstractItemModel>
#include <QItemSelectionModel>

namespace View::Task
{

class PathListModel : public QAbstractListModel
{
    Q_OBJECT

private:
	Model::Task &m_task;

public:
	explicit PathListModel(Model::Task &task, QObject *parent);

	QVariant data(const QModelIndex &index, int role) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;

	QModelIndex movePath(const QModelIndex &index, Model::Task::MoveDirection direction);
	void itemClicked(const QModelIndex &index);

	void updateItemSelection(const Model::Path &path, QItemSelectionModel::SelectionFlag flag, QItemSelectionModel *selectionModel);
	void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
};

}

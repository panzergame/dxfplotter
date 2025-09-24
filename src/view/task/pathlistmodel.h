#pragma once

#include <cassert>

#include <model/task.h>

#include <QAbstractItemModel>
#include <QItemSelectionModel>

namespace view::task
{

class PathListModel : public QAbstractListModel
{
    Q_OBJECT

private:
	model::Task &m_task;
	bool m_ignoreSelectionChanged;

public:
	explicit PathListModel(model::Task &task, QObject *parent);

	QVariant data(const QModelIndex &index, int role) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;

	QModelIndex movePathToDirection(const QModelIndex &index, model::Task::MoveDirection direction);
	QModelIndex movePathToTip(const QModelIndex &index, model::Task::MoveTip tip);
	void itemClicked(const QModelIndex &index);

	void clearSelection(QItemSelectionModel *selectionModel);
	void updateItemSelection(const model::Path &path, QItemSelectionModel::SelectionFlag flag, QItemSelectionModel *selectionModel);
	void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

signals:
	void documentVisibilityChanged();
};

}

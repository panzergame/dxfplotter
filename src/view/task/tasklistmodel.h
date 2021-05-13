#pragma once

#include <cassert>

#include <model/task.h>

#include <QAbstractItemModel>

namespace View::Task
{

class TaskListModel : public QAbstractListModel
{
    Q_OBJECT

private:
	Model::Task *m_task;

public:
	explicit TaskListModel(Model::Task *task, QObject *parent);

	QVariant data(const QModelIndex &index, int role) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QModelIndex movePath(const QModelIndex &index, Model::Task::MoveDirection direction);
	void itemClicked(const QModelIndex &index);
};

}

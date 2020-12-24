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

	virtual QVariant data(const QModelIndex &index, int role) const override;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const override;
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QModelIndex movePath(const QModelIndex &index, Model::Task::MoveDirection direction);
};

}

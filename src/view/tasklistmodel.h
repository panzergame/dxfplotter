#pragma once

#include <model/task.h>

#include <control/application.h>

#include <QAbstractItemModel>

namespace View
{

class TaskListModel : public QAbstractListModel
{
    Q_OBJECT

private:
	const Model::Task *m_task;

public:
	explicit TaskListModel(const Model::Task *task, QObject *parent);

	virtual QVariant data(const QModelIndex &index, int role) const override;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const override;
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
};

}

#include <tasklistmodel.h>

#include <QDebug> // TODO

namespace View
{

TaskListModel::TaskListModel(Model::Task &task, QObject *parent)
	:QAbstractListModel(parent),
	m_task(task)
{
}

QVariant TaskListModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole) {
		if (index.isValid()) {
			Model::Path *path = m_task.pathAt(index.row());
			return QString::fromStdString(path->name());
		}

		return QVariant();
	}

	return QVariant();
}

QVariant TaskListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		return QString("Name");
	}

	return QVariant();
}

int TaskListModel::rowCount(const QModelIndex& parent) const
{
	return m_task.count();
}

}

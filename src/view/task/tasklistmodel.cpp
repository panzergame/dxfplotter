#include <tasklistmodel.h>

#include <QDebug> // TODO

namespace View
{

TaskListModel::TaskListModel(Model::Task *task, QObject *parent)
	:QAbstractListModel(parent),
	m_task(task)
{
}

QVariant TaskListModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole && index.isValid()) {
		if (index.column() == 0) {
			Model::Path *path = m_task->pathAt(index.row());
			return QString::fromStdString(path->name());
		}
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
	return m_task->count();
}

int TaskListModel::columnCount(const QModelIndex& parent) const
{
	return 2;
}

QModelIndex TaskListModel::movePath(const QModelIndex &index, Model::Task::MoveDirection direction)
{
	const int row = index.row();
	const int newRow = row + direction;

	if (index.isValid() && 0 <= newRow && newRow < rowCount(index)) {
		// Mistic qt move indexing
		const int newQtRow = (newRow > row) ? newRow + 1 : newRow;

		if (beginMoveRows(index, row, row, index, newQtRow)) {

			m_task->movePath(row, direction);

			endMoveRows();

			return this->index(newRow);
		}
	}

	return index;
}

}

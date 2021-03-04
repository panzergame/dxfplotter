#include <tasklistmodel.h>

namespace View::Task
{

TaskListModel::TaskListModel(Model::Task *task, QObject *parent)
	:QAbstractListModel(parent),
	m_task(task)
{
}

QVariant TaskListModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) {
		return QVariant();
	}

	Model::Path *path = m_task->pathAt(index.row());

	switch (role) {
		case Qt::DisplayRole:
		{
			switch (index.column()) {
				case 0:
				{
					return QString::fromStdString(path->name());
					break;
				}
			}
			break;
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

void TaskListModel::itemClicked(const QModelIndex& index)
{

	switch (index.column()) {
		case 1:
		{
			Model::Path *path = m_task->pathAt(index.row());
			path->toggleVisible();
			
			emit dataChanged(index, index);
		}
	}
}


}

#include <pathlistmodel.h>

#include <QIcon>

namespace View::Task
{

PathListModel::PathListModel(Model::Task &task, QObject *parent)
	:QAbstractListModel(parent),
	m_task(task)
{
}

QVariant PathListModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) {
		return QVariant();
	}

	const Model::Path &path = m_task.pathAt(index.row());

	switch (role) {
		case Qt::DisplayRole:
		{
			switch (index.column()) {
				case 0:
				{
					return QString::fromStdString(path.name());
					break;
				}
			}
			break;
		}
		case Qt::DecorationRole:
		{
			switch (index.column()) {
				case 1:
				{
					if (path.globallyVisible()) {
						return QIcon::fromTheme("object-visible");
					}
					else {
						return QIcon::fromTheme("object-hidden");
					}
					break;
				}
			}
			break;
		}
	}

	return QVariant();
}

int PathListModel::rowCount(const QModelIndex& parent) const
{
	return m_task.pathCount();
}

int PathListModel::columnCount(const QModelIndex& parent) const
{
	return 2;
}

Qt::ItemFlags PathListModel::flags(const QModelIndex &index) const
{
	if (index.column() == 0) {
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	}

	const Model::Path &path = m_task.pathAt(index.row());
	return (path.layer().visible()) ? Qt::ItemIsEnabled : Qt::NoItemFlags;
}

QModelIndex PathListModel::movePath(const QModelIndex &index, Model::Task::MoveDirection direction)
{
	const int row = index.row();
	const int newRow = row + direction;

	if (index.isValid() && 0 <= newRow && newRow < rowCount(index)) {
		// Mistic qt move indexing
		const int newQtRow = (newRow > row) ? newRow + 1 : newRow;

		if (beginMoveRows(index, row, row, index, newQtRow)) {

			m_task.movePath(row, direction);

			endMoveRows();

			return this->index(newRow);
		}
	}

	return index;
}

void PathListModel::itemClicked(const QModelIndex& index)
{
	if ((index.flags() & Qt::ItemIsEnabled) == 0) {
		return;
	}

	switch (index.column()) {
		case 1:
		{
			Model::Path &path = m_task.pathAt(index.row());
			path.toggleVisible();
			
			emit dataChanged(index, index);
		}
	}
}

void PathListModel::updateItemSelection(const Model::Path &path, QItemSelectionModel::SelectionFlag flag, QItemSelectionModel *selectionModel)
{
	const int row = m_task.pathIndexFor(path);
	selectionModel->select(index(row, 0), flag);
}

void PathListModel::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	for (const QModelIndex &index : selected.indexes()) {
		Model::Path &path = m_task.pathAt(index.row());
		path.setSelected(true);
	}

	for (const QModelIndex &index : deselected.indexes()) {
		Model::Path &path = m_task.pathAt(index.row());
		path.setSelected(false);
	}
}

}

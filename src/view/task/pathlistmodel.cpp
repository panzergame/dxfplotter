module;

#include <cassert>
#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include "qnamespace.h"
#include <QIcon>
#include <QtCore/qtmochelpers.h>

export module view.task.pathlistmodel;

import model.task;
import model.path;

export namespace view::task
{

class PathListModel : public QAbstractListModel
{
	Q_OBJECT

private:
	model::Task& m_task;
	bool m_ignoreSelectionChanged;

public:
	explicit PathListModel(model::Task& task, QObject* parent)
		: QAbstractListModel(parent)
		, m_task(task)
		, m_ignoreSelectionChanged(false)
	{
		connect(&m_task, &model::Task::pathOrderChanged, [this] {
			emit layoutChanged();
		});
	}

	QVariant data(const QModelIndex& index, int role) const override
	{
		if (!index.isValid()) {
			return QVariant();
		}

		const model::Path& path = m_task.pathAt(index.row());

		switch (role) {
			case Qt::DisplayRole:
			case Qt::ToolTipRole: {
				switch (index.column()) {
					case 0: {
						return QString::fromStdString(path.name());
						break;
					}
				}
				break;
			}
			case Qt::DecorationRole: {
				switch (index.column()) {
					case 1: {
						if (path.globallyVisible()) {
							return QIcon::fromTheme(":/icons/layer-visible-on.svg");
						} else {
							return QIcon::fromTheme(":/icons/layer-visible-off.svg");
						}
						break;
					}
				}
				break;
			}
		}

		return QVariant();
	}

	int rowCount([[maybe_unused]] const QModelIndex& parent = QModelIndex()) const override
	{
		return m_task.pathCount();
	}

	int columnCount([[maybe_unused]] const QModelIndex& parent = QModelIndex()) const override { return 2; }

	Qt::ItemFlags flags(const QModelIndex& index) const override
	{
		if (index.column() == 0) {
			return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
		}

		const model::Path& path = m_task.pathAt(index.row());
		return (path.parent()->visible()) ? Qt::ItemIsEnabled : Qt::NoItemFlags;
	}

	QModelIndex movePathToDirection(const QModelIndex& index, model::Task::MoveDirection direction)
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

	QModelIndex movePathToTip(const QModelIndex& index, model::Task::MoveTip tip)
	{
		const int row = index.row();
		const int newRow = (tip == model::Task::MoveTip::Top) ? 0 : (rowCount(index) - 1);

		if (index.isValid()) {
			m_task.movePathToTip(row, tip);

			const QModelIndex newIndex = this->index(newRow);
			emit dataChanged(index, newIndex);

			return newIndex;
		}

		return index;
	}

	void itemClicked(const QModelIndex& index)
	{
		if ((index.flags() & Qt::ItemIsEnabled) == 0) {
			return;
		}

		switch (index.column()) {
			case 1: {
				model::Path& path = m_task.pathAt(index.row());
				path.toggleVisible();

				emit dataChanged(index, index);
			}
		}
	}

	void clearSelection(QItemSelectionModel* selectionModel)
	{
		m_ignoreSelectionChanged = true;

		selectionModel->clear();

		m_ignoreSelectionChanged = false;
	}

	void updateItemSelection(const model::Path& path, QItemSelectionModel::SelectionFlag flag,
							 QItemSelectionModel* selectionModel)
	{
		m_ignoreSelectionChanged = true;

		const int row = m_task.pathIndexFor(path);
		selectionModel->select(index(row, 0), flag);

		m_ignoreSelectionChanged = false;
	}

	void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
	{
		if (m_ignoreSelectionChanged) {
			return;
		}

		for (const QModelIndex& index : selected.indexes()) {
			model::Path& path = m_task.pathAt(index.row());
			path.setSelected(true);
		}

		for (const QModelIndex& index : deselected.indexes()) {
			model::Path& path = m_task.pathAt(index.row());
			path.setSelected(false);
		}
	}

signals:
	void documentVisibilityChanged();
};

}

#include "pathlistmodel.moc"

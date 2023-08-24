#include <layertreemodel.h>

#include <QIcon>

namespace view::task
{

LayerTreeModel::LayerTreeModel(model::Task &task, QObject *parent)
	:QAbstractItemModel(parent),
	m_task(task),
	m_ignoreSelectionChanged(false)
{
}

QVariant LayerTreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) {
		return QVariant();
	}

	const model::Renderable &item = *static_cast<const model::Renderable *>(index.internalPointer());

	switch (role) {
		case Qt::DisplayRole:
		case Qt::ToolTipRole:
		{
			switch (index.column()) {
				case 0:
				{
					return QString::fromStdString(item.name());
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
					if (item.visible()) {
						return QIcon::fromTheme(":/icons/layer-visible-on.svg");
					}
					else {
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

QModelIndex LayerTreeModel::index(int row, int column, const QModelIndex& parent) const
{
	model::Renderable *parentItem = static_cast<model::Renderable *>(parent.internalPointer());
	if (model::Layer *parentLayer = dynamic_cast<model::Layer *>(parentItem)) {
		model::Path &path = parentLayer->childrenAt(row);
		return createIndex(row, column, &path);
	}
	else {
		model::Layer &layer = m_task.layerAt(row);
		return createIndex(row, column, &layer);
	}
}

QModelIndex LayerTreeModel::parent(const QModelIndex& index) const
{
	if (!index.isValid()) {
		return QModelIndex();
	}
	
	model::Renderable *item = static_cast<model::Renderable *>(index.internalPointer());
	if (model::Path *path = dynamic_cast<model::Path *>(item)) {
		model::Layer &layer = path->layer();
		const int row = m_task.layerIndexFor(layer);
		return createIndex(row, 0, &layer);
	}

	return QModelIndex();
}

int LayerTreeModel::rowCount(const QModelIndex& parent) const
{
	model::Renderable *parentItem = static_cast<model::Renderable *>(parent.internalPointer());
	if (model::Layer *parentLayer = dynamic_cast<model::Layer *>(parentItem)) {
		return parentLayer->childrenCount();
	}
	else if ([[maybe_unused]] model::Path *parentPath = dynamic_cast<model::Path *>(parentItem)) {
		return 0;
	}
	else {
		return m_task.layerCount();
	}
}

int LayerTreeModel::columnCount([[maybe_unused]] const QModelIndex& parent) const
{
	return 2;
}

Qt::ItemFlags LayerTreeModel::flags(const QModelIndex &index) const
{
	if (index.column() == 0) {
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate;
	}
	return Qt::ItemIsEnabled;
}

void LayerTreeModel::itemClicked(const QModelIndex& index)
{
	switch (index.column()) {
		case 1:
		{
			model::Renderable *item = static_cast<model::Renderable *>(index.internalPointer());
			item->toggleVisible();

			emit documentVisibilityChanged();

			emit dataChanged(index, index);
		}
	}
}

void LayerTreeModel::clearSelection(QItemSelectionModel *selectionModel)
{
	m_ignoreSelectionChanged = true;

	selectionModel->clear();

	m_ignoreSelectionChanged = false;
}

void LayerTreeModel::updateItemSelection(const model::Path &path, QItemSelectionModel::SelectionFlag flag, QItemSelectionModel *selectionModel)
{
	m_ignoreSelectionChanged = true;

	const std::pair<int, int> indices = m_task.layerAndPathIndexFor(path);
	const QModelIndex parentIndex = index(indices.first, 0);
	const QModelIndex childIndex = index(indices.second, 0, parentIndex);
	selectionModel->select(childIndex, flag);

	m_ignoreSelectionChanged = false;
}

void LayerTreeModel::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	if (m_ignoreSelectionChanged) {
		return;
	}

	for (const QModelIndex &index : selected.indexes()) {
		model::Renderable &renderable = *static_cast<model::Renderable *>(index.internalPointer());
		renderable.setSelected(true);
	}

	for (const QModelIndex &index : deselected.indexes()) {
		model::Renderable &renderable = *static_cast<model::Renderable *>(index.internalPointer());
		renderable.setSelected(false);
	}
}

}

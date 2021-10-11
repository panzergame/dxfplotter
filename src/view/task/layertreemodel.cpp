#include <layertreemodel.h>

#include <QIcon>

namespace View::Task
{

LayerTreeModel::LayerTreeModel(Model::Task &task, QObject *parent)
	:QAbstractItemModel(parent),
	m_task(task)
{
}

QVariant LayerTreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) {
		return QVariant();
	}

	const Model::Renderable &item = *static_cast<const Model::Renderable *>(index.internalPointer());

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

QModelIndex LayerTreeModel::index(int row, int column, const QModelIndex& parent) const
{
	Model::Renderable *parentItem = static_cast<Model::Renderable *>(parent.internalPointer());
	if (Model::Layer *parentLayer = dynamic_cast<Model::Layer *>(parentItem)) {
		Model::Path &path = parentLayer->childrenAt(row);
		return createIndex(row, column, &path);
	}
	else {
		Model::Layer &layer = m_task.layerAt(row);
		return createIndex(row, column, &layer);
	}
}

QModelIndex LayerTreeModel::parent(const QModelIndex& index) const
{
	if (!index.isValid()) {
		return QModelIndex();
	}
	
	Model::Renderable *item = static_cast<Model::Renderable *>(index.internalPointer());
	if (Model::Path *path = dynamic_cast<Model::Path *>(item)) {
		Model::Layer &layer = path->layer();
		const int row = m_task.layerIndexFor(layer);
		return createIndex(row, 0, &layer);
	}

	return QModelIndex();
}

int LayerTreeModel::rowCount(const QModelIndex& parent) const
{
	Model::Renderable *parentItem = static_cast<Model::Renderable *>(parent.internalPointer());
	if (Model::Layer *parentLayer = dynamic_cast<Model::Layer *>(parentItem)) {
		return parentLayer->childrenCount();
	}
	else if (Model::Path *parentPath = dynamic_cast<Model::Path *>(parentItem)) {
		return 0;
	}
	else {
		return m_task.layerCount();
	}
}

int LayerTreeModel::columnCount(const QModelIndex& parent) const
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
			Model::Renderable *item = static_cast<Model::Renderable *>(index.internalPointer());
			item->toggleVisible();

			emit dataChanged(index, index);
		}
	}
}

void LayerTreeModel::updateItemSelection(const Model::Path &path, QItemSelectionModel::SelectionFlag flag, QItemSelectionModel *selectionModel)
{
	const std::pair<int, int> indices = m_task.layerAndPathIndexFor(path);
	const QModelIndex parentIndex = index(indices.first, 0);
	const QModelIndex childIndex = index(indices.second, 0, parentIndex);
	selectionModel->select(childIndex, flag);
}

void LayerTreeModel::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	for (const QModelIndex &index : selected.indexes()) {
		Model::Renderable &renderable = *static_cast<Model::Renderable *>(index.internalPointer());
		renderable.setSelected(true);
	}

	for (const QModelIndex &index : deselected.indexes()) {
		Model::Renderable &renderable = *static_cast<Model::Renderable *>(index.internalPointer());
		renderable.setSelected(false);
	}
}

}

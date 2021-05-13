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

void LayerTreeModel::itemClicked(const QModelIndex& index)
{

	/*switch (index.column()) {
		case 1:
		{
			Model::Path &path = m_task.pathAt(index.row());
			path.toggleVisible();
			
			emit dataChanged(index, index);
		}
	}*/
}

void LayerTreeModel::updateItemSelection(const Model::Path &path, QItemSelectionModel::SelectionFlag flag, QItemSelectionModel *selectionModel)
{
// 	const int row = m_task.indexFor(path);
// 	selectionModel->select(index(row, 0), flag);
}

void LayerTreeModel::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	/*for (const QModelIndex &index : selected.indexes()) {
		Model::Path &path = m_task.pathAt(index.row());
		path.setSelected(true);
	}

	for (const QModelIndex &index : deselected.indexes()) {
		Model::Path &path = m_task.pathAt(index.row());
		path.setSelected(false);
	}*/
}

}

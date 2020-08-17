#include <settingtreemodel.h>

#include <QDebug> // TODO

namespace View
{

void SettingTreeModel::constructNodes()
{
	m_root = {0, &m_configRoot, nullptr, {}};

	constructChildren(m_root);
}

void SettingTreeModel::constructChildren(Node &parent)
{
	// Row counter
	int row = 0;

	// First create all children
	parent.configNode->visitChildren([&parent, &row](Config::Node &node){
		// Create node linked to config node
		Node child = {row, static_cast<Config::NodeList *>(&node), &parent, {}};

		parent.children.push_back(child);
	});

	// Second go recursively
	// Test if node is a group
	Config::Group *parentGroup = dynamic_cast<Config::Group *>(parent.configNode);
	if (parentGroup) {
		for (Node &node : parent.children) {
			constructChildren(node);
		}
	}
}

SettingTreeModel::SettingTreeModel(Config::Group &root, QObject *parent)
	:QAbstractItemModel(parent),
	m_configRoot(root)
{
	constructNodes();
}

QVariant SettingTreeModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole && index.isValid()) {
		Node *node = static_cast<Node *>(index.internalPointer());

		return QString::fromStdString(node->configNode->description());
	}

	return QVariant();
}

Qt::ItemFlags SettingTreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid()) {
		return Qt::NoItemFlags;
	}

	return QAbstractItemModel::flags(index);
}

QVariant SettingTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		return QString("Name");
	}

	return QVariant();
}

QModelIndex SettingTreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent)) {
		return QModelIndex();
	}

	Node *parentNode = !parent.isValid() ? const_cast<Node *>(&m_root) : static_cast<Node *>(parent.internalPointer());

	// Test if node is a group
	Config::Group *parentGroup = dynamic_cast<Config::Group *>(parentNode->configNode);
	if (parentGroup) {
		return createIndex(row, column, &parentNode->children[row]);
	}

	return QModelIndex();
}

QModelIndex SettingTreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid()) {
		return QModelIndex();
	}

	Node *node = static_cast<Node *>(index.internalPointer());

	// Reached top level
	if (node == &m_root) {
		return QModelIndex();
	}

	Node *parentNode = node->parent;

	return createIndex(parentNode->row, 0, parentNode);
}

int SettingTreeModel::rowCount(const QModelIndex& parent) const
{
	if (parent.isValid()) {
		Node *node = static_cast<Node *>(parent.internalPointer());

		return node->configNode->size();
	}

	return m_root.configNode->size();
}

int SettingTreeModel::columnCount(const QModelIndex& parent) const
{
	return 1;
}

}

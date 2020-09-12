#include <settingtreemodel.h>

#include <QDebug> // TODO

namespace View
{

class SettingTreeModel::ConstructorVisitor
{
private:
	Node *m_parent;
	int m_row;

public:
	explicit ConstructorVisitor(Node *parent)
		:m_parent(parent),
		m_row(0)
	{
	}

	template <class ConfigNode>
	void operator()(ConfigNode &node)
	{
		// Create node linked to config node
		Node *n = new Node{m_row++, &node, m_parent, {}};
		m_parent->children.emplace_back(n);

		// First create all children
		ConstructorVisitor visitor(n);
		node.visitChildren(visitor);
	}

	template <class ValueType>
	void operator()(Config::Property<ValueType> &)
	{
	}
};

void SettingTreeModel::constructNodes()
{
	// Construct root node
	m_root = {0, &m_configRoot, nullptr, {}};
	ConstructorVisitor visitor(&m_root);
	m_configRoot.visitChildren(visitor);
}

SettingTreeModel::SettingTreeModel(Config::Root &root, QObject *parent)
	:QAbstractItemModel(parent),
	m_configRoot(root)
{
	constructNodes();
}

QVariant SettingTreeModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole && index.isValid()) {
		Node *node = static_cast<Node *>(index.internalPointer());

		const std::string &name = std::visit([](const auto& node){return node->name();},
				node->configNode);

		return QString::fromStdString(name);
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

QModelIndex SettingTreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent)) {
		return QModelIndex();
	}

	const Node *parentNode = !parent.isValid() ? &m_root : static_cast<const Node *>(parent.internalPointer());

	// Test if node is a group
	return createIndex(row, column, parentNode->children[row].get());
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

		return node->children.size();
	}

	return m_root.children.size();
}

int SettingTreeModel::columnCount(const QModelIndex &) const
{
	return 1;
}

}

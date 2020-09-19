#include <treemodel.h>

#include <QDebug> // TODO

namespace View::Settings
{

struct TreeModel::ConstructorVisitor
{
	Node *parent;
	int row = 0;

	template <class ValueType>
	void operator()(Config::Property<ValueType> &)
	{
	}

	template <class ConfigNode>
	void operator()(ConfigNode &node)
	{
		// Create node linked to config node
		Node *n = new Node{row++, nodeType(node), &node, parent, {}};
		parent->children.emplace_back(n);

		// First create all children
		ConstructorVisitor visitor{n};
		node.visitChildren(visitor);
	}
};

struct TreeModel::AddItemVisitor
{
	const QString &name;
	const int row;
	Node *parent;

	template <class Child>
	void operator()(Config::List<Child> *list)
	{
		qInfo() << "add";
		Child &child = list->createChild(name.toStdString());
		Node *n = new Node{row, nodeType(child), &child, parent, {}};
	}

	template <class ... Child>
	void operator()(Config::Group<Child ...> *)
	{
	}
};

void TreeModel::constructNodes()
{
	// Construct root node
	m_root = {0, Node::Type::Group, &m_configRoot, nullptr, {}};
	ConstructorVisitor visitor{&m_root};
	m_configRoot.visitChildren(visitor);
}

TreeModel::TreeModel(Config::Root &root, QObject *parent)
	:QAbstractItemModel(parent),
	m_configRoot(root)
{
	constructNodes();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole && index.isValid() && index.column() == 0) {
		Node *node = static_cast<Node *>(index.internalPointer());

		const std::string &name = std::visit([](const auto& node){return node->name();},
				node->configNode);

		return QString::fromStdString(name);
	}

	return QVariant();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid()) {
		return Qt::NoItemFlags;
	}

	return QAbstractItemModel::flags(index);
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent)) {
		return QModelIndex();
	}

	const Node *parentNode = !parent.isValid() ? &m_root : static_cast<const Node *>(parent.internalPointer());

	return createIndex(row, column, parentNode->children[row].get());
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
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

int TreeModel::rowCount(const QModelIndex& parent) const
{
	if (parent.isValid()) {
		Node *node = static_cast<Node *>(parent.internalPointer());

		return node->children.size();
	}

	return m_root.children.size();
}

int TreeModel::columnCount(const QModelIndex &) const
{
	return 1;
}

bool TreeModel::isList(const QModelIndex &index) const
{
	Node *node = static_cast<Node *>(index.internalPointer());
	return (node->type == Node::Type::List);
}

bool TreeModel::isItem(const QModelIndex &index) const
{
	Node *node = static_cast<Node *>(index.internalPointer());
	return (node->parent && node->parent->type == Node::Type::List);
}

void TreeModel::addItem(const QModelIndex &parent, const QString &name)
{
	assert(isList(parent));

	const int row = rowCount(parent);
	beginInsertRows(parent, row, row);

	Node *node = static_cast<Node *>(parent.internalPointer());

	std::visit(AddItemVisitor{name, row, node}, node->configNode);

	endInsertRows();
}

void TreeModel::removeItem(const QModelIndex &index)
{
	
}

}

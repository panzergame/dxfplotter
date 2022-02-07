#include <treemodel.h>

#include <QDebug> // TODO

namespace view::settings
{

struct TreeModel::ConstructorItemVisitor
{
	Node &parent;
	// Row of next generated child node
	int row = 0;

	template <class ValueType>
	void operator()(config::Property<ValueType> &)
	{
	}

	template <class ConfigNode>
	void operator()(ConfigNode &node)
	{
		// Create node linked to config node
		Node::UPtr n = std::make_unique<Node>(row++, node, &parent);

		// Create all children
		ConstructorItemVisitor visitor{*n};
		node.visitChildren(visitor);

		parent.children.push_back(std::move(n));
	}
};

struct TreeModel::AddItemVisitor
{
	const QString &name;
	Node &parent;

	template <class Child>
	void operator()(config::List<Child> *list)
	{
		const int nextRow = static_cast<int>(parent.children.size());

		Child &child = list->createChild(name.toStdString());

		Node::UPtr n = std::make_unique<Node>(nextRow, child, &parent);

		// Create all children
		ConstructorItemVisitor visitor{*n};
		child.visitChildren(visitor);

		parent.children.push_back(std::move(n));
	}

	template <class ... Child>
	void operator()(config::Group<Child ...> *)
	{
	}
};

struct TreeModel::RemoveItemVisitor
{
	const config::Node &configNode;

	template <class Child>
	void operator()(config::List<Child> *list)
	{
		list->removeChild(static_cast<const Child &>(configNode));
	}

	template <class ... Child>
	void operator()(config::Group<Child ...> *)
	{
	}
};

struct TreeModel::CopyItemVisitor
{
	const QString &name;
	Node &parent;
	Node &source;

	template <class Child>
	void operator()(config::List<Child> *list)
	{
		const int nextRow = parent.children.size();

		const Child &sourceChild = *std::get<Child *>(source.configNode);
		Child &child = list->copyChild(sourceChild, name.toStdString());

		Node::UPtr n = std::make_unique<Node>(nextRow, child, &parent);

		// Create all children
		ConstructorItemVisitor visitor{*n};
		child.visitChildren(visitor);

		parent.children.push_back(std::move(n));
	}

	template <class ... Child>
	void operator()(config::Group<Child ...> *)
	{
	}
};

void TreeModel::constructNodes()
{
	// Construct root node
	m_root = Node(0, m_configRoot, nullptr);
	ConstructorItemVisitor visitor{m_root};
	m_configRoot.visitChildren(visitor);
}

TreeModel::TreeModel(config::Root &root, QObject *parent)
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

	if (parent.isValid()) {
		const Node *parentNode = static_cast<const Node *>(parent.internalPointer());
		const Node* childNode = parentNode->children[row].get();
		return createIndex(row, column, const_cast<Node *>(childNode));
	}
	return createIndex(row, column, const_cast<Node*>(&m_root));

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

void TreeModel::addItem(const QModelIndex &parent, const QString &newName)
{
	assert(isList(parent));

	const int row = rowCount(parent);
	beginInsertRows(parent, row, row);

	Node &node = *static_cast<Node *>(parent.internalPointer());

	std::visit(AddItemVisitor{newName, node}, node.configNode);

	endInsertRows();
}

void TreeModel::removeItem(const QModelIndex &index)
{
	assert(isItem(index));

	const QModelIndex &parent = index.parent();
	const int row = index.row();

	Node &parentNode = *static_cast<Node *>(parent.internalPointer());
	Node &node = *static_cast<Node *>(index.internalPointer());

	beginRemoveRows(parent, row, row);

	// Remove item in config list
	std::visit([&parentNode](auto *configNode){
		std::visit(RemoveItemVisitor{*configNode}, parentNode.configNode);
	}, node.configNode);

	// Remove child and retrieve position after.
	auto it = parentNode.children.erase(parentNode.children.begin() + row);

	// Remap row indices (-1)
	for (const auto &end = parentNode.children.end(); it != end; ++it) {
		--(*it)->row;
	}

	endRemoveRows();
}

void TreeModel::copyItem(const QModelIndex &index, const QString &newName)
{
	const QModelIndex &parent = index.parent();
	assert(isList(parent));

	const int row = rowCount(parent);
	beginInsertRows(parent, row, row);

	Node &parentNode = *static_cast<Node *>(parent.internalPointer());
	Node &node = *static_cast<Node *>(index.internalPointer());

	std::visit(CopyItemVisitor{newName, parentNode, node}, parentNode.configNode);

	endInsertRows();
}

}

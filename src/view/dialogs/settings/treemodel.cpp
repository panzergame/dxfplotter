module;

#include <QAbstractItemModel>
#include <QDebug>
#include <QtCore/qtmochelpers.h>

export module view.dialogs.settings.treemodel;

import common.aggregable;
import config.config;
import config.group;
import config.list;
import config.node;
import config.property;

export namespace view::settings
{

class TreeModel : public QAbstractItemModel
{
	Q_OBJECT

private:
	// ItemModel requests each item to know its parent and also its row in parent
	struct Node : common::Aggregable<Node>
	{
		int row;
		enum class Type { Group, List } type;

		config::NodePtrVariant configNode;
		Node* parent;
		ListUPtr children;

		template<class... Child>
		static constexpr Node::Type nodeType(const config::Group<Child...>&)
		{
			return Node::Type::Group;
		}

		template<class Child>
		static constexpr Node::Type nodeType(const config::List<Child>&)
		{
			return Node::Type::List;
		};

		Node() = default;

		template<class ConfigNode>
		explicit Node(int _row, ConfigNode& _node, Node* _parent)
			: row(_row)
			, type(nodeType(_node))
			, configNode(&_node)
			, parent(_parent)
		{
		}
	};

	config::Root& m_configRoot;
	Node m_root;

	struct ConstructorItemVisitor
	{
		Node& parent;
		// Row of next generated child node
		int row = 0;

		template<class ValueType>
		void operator()(config::Property<ValueType>&)
		{
		}

		template<class ConfigNode>
		void operator()(ConfigNode& node)
		{
			// Create node linked to config node
			Node::UPtr n = std::make_unique<Node>(row++, node, &parent);

			// Create all children
			ConstructorItemVisitor visitor { *n };
			node.visitChildren(visitor);

			parent.children.push_back(std::move(n));
		}
	};

	struct AddItemVisitor
	{
		const QString& name;
		Node& parent;

		template<class Child>
		void operator()(config::List<Child>* list)
		{
			const int nextRow = static_cast<int>(parent.children.size());

			Child& child = list->createChild(name.toStdString());

			Node::UPtr n = std::make_unique<Node>(nextRow, child, &parent);

			// Create all children
			ConstructorItemVisitor visitor { *n };
			child.visitChildren(visitor);

			parent.children.push_back(std::move(n));
		}

		template<class... Child>
		void operator()(config::Group<Child...>*)
		{
		}
	};

	struct RemoveItemVisitor
	{
		const config::Node& configNode;

		template<class Child>
		void operator()(config::List<Child>* list)
		{
			list->removeChild(static_cast<const Child&>(configNode));
		}

		template<class... Child>
		void operator()(config::Group<Child...>*)
		{
		}
	};

	struct CopyItemVisitor
	{
		const QString& name;
		Node& parent;
		Node& source;

		template<class Child>
		void operator()(config::List<Child>* list)
		{
			const int nextRow = parent.children.size();

			const Child& sourceChild = *std::get<Child*>(source.configNode);
			Child& child = list->copyChild(sourceChild, name.toStdString());

			Node::UPtr n = std::make_unique<Node>(nextRow, child, &parent);

			// Create all children
			ConstructorItemVisitor visitor { *n };
			child.visitChildren(visitor);

			parent.children.push_back(std::move(n));
		}

		template<class... Child>
		void operator()(config::Group<Child...>*)
		{
		}
	};

	void constructNodes()
	{
		// Construct root node
		m_root = Node(0, m_configRoot, nullptr);
		ConstructorItemVisitor visitor { m_root };
		m_configRoot.visitChildren(visitor);
	}

public:
	explicit TreeModel(config::Root& root, QObject* parent = nullptr)
		: QAbstractItemModel(parent)
		, m_configRoot(root)
	{
		constructNodes();
	}

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override
	{
		if (role == Qt::DisplayRole && index.isValid() && index.column() == 0) {
			Node* node = static_cast<Node*>(index.internalPointer());

			const std::string& name = std::visit(
				[](const auto& node) {
					return node->name();
				},
				node->configNode);

			return QString::fromStdString(name);
		}

		return QVariant();
	}

	Qt::ItemFlags flags(const QModelIndex& index) const override
	{
		if (!index.isValid()) {
			return Qt::NoItemFlags;
		}

		return QAbstractItemModel::flags(index);
	}

	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override
	{
		if (!hasIndex(row, column, parent)) {
			return QModelIndex();
		}

		if (parent.isValid()) {
			const Node* parentNode = static_cast<const Node*>(parent.internalPointer());
			const Node* childNode = parentNode->children[row].get();
			return createIndex(row, column, const_cast<Node*>(childNode));
		}
		return createIndex(row, column, const_cast<Node*>(&m_root));
	}

	QModelIndex parent(const QModelIndex& index) const override
	{
		if (!index.isValid()) {
			return QModelIndex();
		}

		Node* node = static_cast<Node*>(index.internalPointer());

		// Reached top level
		if (node == &m_root) {
			return QModelIndex();
		}

		Node* parentNode = node->parent;

		return createIndex(parentNode->row, 0, parentNode);
	}

	int rowCount(const QModelIndex& parent = QModelIndex()) const override
	{
		if (parent.isValid()) {
			Node* node = static_cast<Node*>(parent.internalPointer());

			return node->children.size();
		}

		return 1;
	}

	int columnCount(const QModelIndex& = QModelIndex()) const override { return 1; }

	//// Visit configuration node at index
	template<class Visitor>
	void visit(const QModelIndex& index, Visitor&& visitor) const
	{
		Node* node = static_cast<Node*>(index.internalPointer());

		std::visit(
			[&visitor](auto& node) {
				visitor(*node);
			},
			node->configNode);
	}

	/// Check if index contains a config list
	bool isList(const QModelIndex& index) const
	{
		Node* node = static_cast<Node*>(index.internalPointer());
		return (node->type == Node::Type::List);
	}

	/// Check if index contains a config item (parent is a list)
	bool isItem(const QModelIndex& index) const
	{
		Node* node = static_cast<Node*>(index.internalPointer());
		return (node->parent && node->parent->type == Node::Type::List);
	}

	void addItem(const QModelIndex& parent, const QString& newName)
	{
		assert(isList(parent));

		const int row = rowCount(parent);
		beginInsertRows(parent, row, row);

		Node& node = *static_cast<Node*>(parent.internalPointer());

		std::visit(AddItemVisitor { newName, node }, node.configNode);

		endInsertRows();
	}

	void removeItem(const QModelIndex& index)
	{
		assert(isItem(index));

		const QModelIndex& parent = index.parent();
		const int row = index.row();

		Node& parentNode = *static_cast<Node*>(parent.internalPointer());
		Node& node = *static_cast<Node*>(index.internalPointer());

		beginRemoveRows(parent, row, row);

		// Remove item in config list
		std::visit(
			[&parentNode](auto* configNode) {
				std::visit(RemoveItemVisitor { *configNode }, parentNode.configNode);
			},
			node.configNode);

		// Remove child and retrieve position after.
		auto it = parentNode.children.erase(parentNode.children.begin() + row);

		// Remap row indices (-1)
		for (const auto& end = parentNode.children.end(); it != end; ++it) {
			--(*it)->row;
		}

		endRemoveRows();
	}

	void copyItem(const QModelIndex& index, const QString& newName)
	{
		const QModelIndex& parent = index.parent();
		assert(isList(parent));

		const int row = rowCount(parent);
		beginInsertRows(parent, row, row);

		Node& parentNode = *static_cast<Node*>(parent.internalPointer());
		Node& node = *static_cast<Node*>(index.internalPointer());

		std::visit(CopyItemVisitor { newName, parentNode, node }, parentNode.configNode);

		endInsertRows();
	}
};

}

#include "treemodel.moc"

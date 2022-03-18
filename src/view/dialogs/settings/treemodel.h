#pragma once

#include <QAbstractItemModel>

#include <config/config.h>
#include <common/aggregable.h>

namespace view::settings
{

class TreeModel : public QAbstractItemModel
{
	Q_OBJECT

private:
	struct ConstructorItemVisitor;
	struct AddItemVisitor;
	struct RemoveItemVisitor;
	struct CopyItemVisitor;

	// ItemModel requests each item to know its parent and also its row in parent
	struct Node : common::Aggregable<Node>
	{
		int row;
		enum class Type
		{
			Group,
			List
		} type;

		config::NodePtrVariant configNode;
		Node *parent;
		ListUPtr children;

		template <class ... Child>
		static constexpr Node::Type nodeType(const config::Group<Child...> &)
		{
			return Node::Type::Group;
		}

		template <class Child>
		static constexpr Node::Type nodeType(const config::List<Child> &)
		{
			return Node::Type::List;
		};

		Node() = default;

		template <class ConfigNode>
		explicit Node(int _row, ConfigNode &_node, Node *_parent)
			:row(_row),
			type(nodeType(_node)),
			configNode(&_node),
			parent(_parent)
		{
		}
	};

	config::Root &m_configRoot;
	Node m_root;

	void constructNodes();

public:
	explicit TreeModel(config::Root &root, QObject *parent = nullptr);

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	//// Visit configuration node at index
	template <class Visitor>
	void visit(const QModelIndex &index, Visitor &&visitor) const
	{
		Node *node = static_cast<Node *>(index.internalPointer());

		std::visit([&visitor](auto &node){
			visitor(*node);
		}, node->configNode);
	}

	/// Check if index contains a config list
	bool isList(const QModelIndex &index) const;
	/// Check if index contains a config item (parent is a list)
	bool isItem(const QModelIndex &index) const;

	void addItem(const QModelIndex &parent, const QString &newName);
	void removeItem(const QModelIndex &index);
	void copyItem(const QModelIndex &index, const QString &newName);
};

}

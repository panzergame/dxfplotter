#pragma once

#include <QAbstractItemModel>

#include <config/config.h>

namespace View::Settings
{

class TreeModel : public QAbstractItemModel
{
	Q_OBJECT

private:
	struct ConstructorVisitor;
	struct AddItemVisitor;

	// ItemModel requests each item to know its parent and also its row in parent
	struct Node
	{
		int row;
		enum class Type
		{
			Group,
			List
		} type;

		Config::NodePtrVariant configNode;
		Node *parent;
		std::vector<std::unique_ptr<Node>> children;
	};

	template <class ... Child>
	static constexpr Node::Type nodeType(Config::Group<Child...> &)
	{
		return Node::Type::Group;
	}

	template <class Child>
	static constexpr Node::Type nodeType(Config::List<Child> &)
	{
		return Node::Type::List;
	};

	Config::Root &m_configRoot;
	Node m_root;

	void constructNodes();

public:
	explicit TreeModel(Config::Root &root, QObject *parent = nullptr);

	QVariant data(const QModelIndex &index, int role) const override;
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

	bool isList(const QModelIndex &index) const;
	bool isItem(const QModelIndex &index) const;

	void addItem(const QModelIndex &parent, const QString &name);
	void removeItem(const QModelIndex &index);
};

}

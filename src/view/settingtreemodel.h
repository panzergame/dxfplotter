#pragma once

#include <QAbstractItemModel>

#include <config/config.h>

namespace View
{

class SettingTreeModel : public QAbstractItemModel
{
	Q_OBJECT

private:
	class ConstructorVisitor;

	// ItemModel requests each item to know its parent and also its row in parent
	struct Node
	{
		int row;
		Config::NodePtrVariant configNode;
		Node *parent;
		std::vector<std::unique_ptr<Node>> children;
	};

	Config::Root &m_configRoot;
	Node m_root;

	void constructNodes();

public:
	explicit SettingTreeModel(Config::Root &root, QObject *parent = nullptr);

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
};

}

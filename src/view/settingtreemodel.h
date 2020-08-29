#pragma once

#include <QAbstractItemModel>

#include <config/config.h>

namespace View
{

class SettingTreeModel : public QAbstractItemModel
{
	Q_OBJECT

private:
	Config::Group &m_configRoot;

	// ItemModel requests each item to know its parent and also its row in parent
	struct Node
	{
		int row;
		Config::NodeList *configNode;
		Node *parent;
		std::vector<Node> children;
	};

	Node m_root;

	void constructNodes();
	void constructChildren(Node &parent);

public:
	explicit SettingTreeModel(Config::Group &root, QObject *parent = nullptr);

	QVariant data(const QModelIndex &index, int role) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	//// Return section of the index, otherwise nullptr
	Config::Section *section(const QModelIndex &index) const;
};

}

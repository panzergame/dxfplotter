#pragma once

#include <uic/dialogs/settings/ui_list.h>

#include <QGroupBox>
#include <QAbstractListModel>

namespace view::settings
{

class List : public QGroupBox, private Ui::List
{
public:
	template <class Node>
	explicit List(Node &node)
	{
		setupUi(this);

		setTitle(QString::fromStdString(node.name()));
	}
};

}

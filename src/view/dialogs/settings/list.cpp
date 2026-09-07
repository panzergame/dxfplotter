module;

#include <uic/dialogs/settings/ui_list.h>
#include <QGroupBox>
#include <QAbstractListModel>

export module view.dialogs.settings.list;

export namespace view::settings
{

class List : public QGroupBox, private Ui::List
{
public:
	template<class Node>
	explicit List(Node& node)
	{
		setupUi(this);

		setTitle(QString::fromStdString(node.name()));
	}
};

}

namespace view::settings
{

}

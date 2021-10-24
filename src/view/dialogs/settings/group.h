#pragma once

#include <uic/settings/ui_group.h>

#include <view/dialogs/settings/entry.h>

#include <QGroupBox>

namespace View::Settings
{

class Group : public QGroupBox, private Ui::Group
{
private:
	/// Node visitor populating form layout with properties if any
	class PropertyVisitor
	{
	private:
		Group &m_parent;

	public:
		explicit PropertyVisitor(Group &parent)
			:m_parent(parent)
		{
		}

		template <class ValueType>
		void operator()(Config::Property<ValueType> &property)
		{
			Entry<ValueType> *entry = new Entry<ValueType>(property, &m_parent);

			m_parent.formLayout->addRow(QString::fromStdString(property.name()), entry);
		}

		template <class Node>
		void operator()(Node &)
		{
		}
	};

public:
	template <class Node>
	explicit Group(Node &node)
	{
		setupUi(this);

		setTitle(QString::fromStdString(node.name()));
		// Populate properties
		node.visitChildren(PropertyVisitor(*this));
	}
};

}

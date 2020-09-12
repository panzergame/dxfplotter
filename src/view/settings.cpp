#include <settings.h>
#include <settingtreemodel.h>

#include <QTabWidget>
#include <QFormLayout>

#include <QDebug> // TODO

namespace View
{

void Settings::setupUi()
{
	Ui::Settings::setupUi(this);

	treeView->setModel(m_model.get());
// 	treeView->expandAll();
}

void Settings::save()
{
	for (ISettingEntry *entry : m_entries) {
		entry->save();
	}

	// Ensure configuration is saved to file
	m_config.save();
}

Settings::Settings(Config::Config &config)
	:m_config(config),
	m_model(new SettingTreeModel(m_config.root(), this))
{
	setupUi();

	connect(treeView, &QTreeView::clicked, this, &Settings::clicked);
	connect(this, &QDialog::accepted, this, &Settings::save);
}

/// Node visitor populating form layout with properties if any
class PropertyVisitor
{
private:
	QFormLayout *m_formLayout;
	QWidget *m_parent;
	ISettingEntry::ListPtr &m_entries;

public:
	explicit PropertyVisitor(QFormLayout *formLayout, QWidget *parent, ISettingEntry::ListPtr &entries)
		:m_formLayout(formLayout),
		m_parent(parent),
		m_entries(entries)
	{
	}

	template <class ValueType>
	void operator()(Config::Property<ValueType> &property)
	{
		SettingEntry<ValueType> *entry = new SettingEntry<ValueType>(property, m_parent);

		m_formLayout->addRow(QString::fromStdString(property.name()), entry);
		m_entries.push_back(entry);
	}

	template <class Node>
	void operator()(Node &)
	{
	}
};

void Settings::clicked(const QModelIndex &index)
{
	// Save previous values
	save();

	// Clear all entries, they are destructed when removing form rows
	m_entries.clear();

	// Delete all items
	for (int i = 0, size = formLayout->rowCount(); i < size; ++i) {
		formLayout->removeRow(0);
	}

	m_model->visit(index, [this](auto &node){
		// Set title
		center->setTitle(QString::fromStdString(node.name()));
		// Populate properties
		node.visitChildren(PropertyVisitor(formLayout, center, m_entries));
	});
}

}

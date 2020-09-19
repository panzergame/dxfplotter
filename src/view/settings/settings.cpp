#include <settings.h>
#include <treemodel.h>
#include <group.h>
#include <list.h>

#include <QVBoxLayout>
#include <QInputDialog>
#include <QDebug> // TODO

namespace View::Settings
{

/// Node visitor selecting list UI or group UI
class Settings::NodeVisitor
{
private:
	QWidget *m_newWidget;

public:
	template <class ... Child>
	void operator()(Config::Group<Child ...> &node)
	{
		m_newWidget = new Group(node);
	}

	template <class Child>
	void operator()(Config::List<Child> &node)
	{
		m_newWidget = new List(node);
	}

	QWidget *newWidget() const
	{
		return m_newWidget;
	}
};

void Settings::setupUi()
{
	Ui::Settings::setupUi(this);

	treeView->setModel(m_model.get());
	treeView->expandAll();
	treeView->resizeColumnToContents(0);
}

Settings::Settings(Model::Application &app)
	:m_app(app),
	m_config(app.config()),
	m_model(new TreeModel(app.config().root(), this))
{
	setupUi();

	connect(treeView, &QTreeView::clicked, this, &Settings::clicked);
}

void Settings::accept()
{
	// Set modified config back
	m_app.setConfig(std::move(m_config));

	QDialog::accept();
}

void Settings::clicked(const QModelIndex &index)
{
	// Create new widget to be displayed at center
	NodeVisitor visitor;
	m_model->visit(index, visitor);

	// Replace old center widget
	QWidget *newWidget = visitor.newWidget();
	QLayoutItem *item = gridLayout->replaceWidget(center, newWidget);
	center = newWidget;

	// Delete old center widget
	delete item->widget();

	const bool isList = m_model->isList(index);
	if (isList) {
		connect(addButton, &QPushButton::pressed, this, [index, this](){ addItem(index); });
	}
	else {
		disconnect(addButton);
	}

	addButton->setEnabled(isList);

	const bool isItem = m_model->isItem(index);
	if (isItem) {
		connect(removeButton, &QPushButton::pressed, this, [index, this](){ removeItem(index); });
	}
	else {
		disconnect(removeButton);
	}
	removeButton->setEnabled(isItem);
}

void Settings::addItem(const QModelIndex &index)
{
	bool ok;
	const QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
		tr("Tool name:"), QLineEdit::Normal, "", &ok);

	if (ok && !text.isEmpty()) {
		m_model->addItem(index, text);
	}
}

void Settings::removeItem(const QModelIndex &index)
{
	m_model->removeItem(index);
}

}

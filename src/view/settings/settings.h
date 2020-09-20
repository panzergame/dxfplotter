#pragma once


#include <config/config.h>

#include <model/application.h>

#include <view/settings/entry.h>

#include <uic/settings/ui_settings.h>

#include <QDialog>

namespace View::Settings
{

class TreeModel;

class Settings : public QDialog, private Ui::Settings
{
private:
	class NodeVisitor;

	void setupUi();

	Model::Application &m_app;
	// Modified config
	Config::Config m_newConfig;
	std::unique_ptr<TreeModel> m_model;

public:
	explicit Settings(Model::Application &app);
	~Settings();

	Config::Config &&newConfig();

protected Q_SLOTS:
	void currentChanged(const QModelIndex &index, const QModelIndex &previous);
	void addItem(const QModelIndex &index);
	void removeItem(const QModelIndex &index);
};

}

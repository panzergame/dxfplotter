#pragma once

#include <config/config.h>

#include <view/settingentry.h>

#include <ui_settings.h>

#include <QDialog>

namespace View
{

class SettingTreeModel;

class Settings : public QDialog, private Ui::Settings
{
private:
	Config::Config &m_config;
	ISettingEntry::ListPtr m_entries;

	std::unique_ptr<SettingTreeModel> m_model;

	void setupUi();

	/// Save all entries to configuration
	void save();

public:
	explicit Settings(Config::Config &config);

};

}

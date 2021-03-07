#pragma once

#include <uic/ui_profile.h>

#include <model/application.h>

namespace View
{

class Profile : public QWidget, public Ui::Profile
{
private:
	Model::Application &m_app;

	bool m_outsideToolChangeBlocked;

	void updateTools(const Config::Tools &tools);

public:
	explicit Profile(Model::Application &app);

public Q_SLOTS:
	void configChanged(const Config::Config &config);
	void selectedToolConfigChanged(const Config::Tools::Tool &tool);
	void currentToolTextChanged(const QString &toolName);
};

}

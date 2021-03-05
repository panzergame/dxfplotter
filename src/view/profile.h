#pragma once

#include <uic/ui_profile.h>

#include <model/application.h>

namespace View
{

class Profile : public QWidget, public Ui::Profile
{
private:
	void updateToolSelector(const Config::Config &config);

public:
	explicit Profile(Model::Application &app);

public Q_SLOTS:
	void configChanged(const Config::Config &config);
};

}

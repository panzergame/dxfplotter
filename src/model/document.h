#pragma once

#include <model/task.h>
#include <config/config.h>

namespace Model
{

class Document : public QObject, public Common::Aggregable<Document>
{
	Q_OBJECT;

private:
	Task::UPtr m_task;
	const Config::Tools::Tool *m_toolConfig;
	const Config::Profiles::Profile *m_profileConfig;

public:
	explicit Document(Task::UPtr&& task, const Config::Tools::Tool &toolConfig, const Config::Profiles::Profile &profileConfig);
	Document() = default;

	Task& task();
	const Task& task() const;

	const Config::Tools::Tool &toolConfig() const;
	const Config::Profiles::Profile &profileConfig() const;
	void setToolConfig(const Config::Tools::Tool &tool);
	void setProfileConfig(const Config::Profiles::Profile &profile);

Q_SIGNALS:
	void toolConfigChanged(const Config::Tools::Tool &tool);
	void profileConfigChanged(const Config::Profiles::Profile &profile);
};

}

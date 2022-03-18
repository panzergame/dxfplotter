#pragma once

#include <model/task.h>
#include <config/config.h>

namespace model
{

class Document : public QObject, public common::Aggregable<Document>
{
	Q_OBJECT;

private:
	Task::UPtr m_task;
	const config::Tools::Tool *m_toolConfig;
	const config::Profiles::Profile *m_profileConfig;

public:
	explicit Document(Task::UPtr&& task, const config::Tools::Tool &toolConfig, const config::Profiles::Profile &profileConfig);
	Document() = default;

	Task& task();
	const Task& task() const;

	const config::Tools::Tool &toolConfig() const;
	const config::Profiles::Profile &profileConfig() const;
	void setToolConfig(const config::Tools::Tool &tool);
	void setProfileConfig(const config::Profiles::Profile &profile);

Q_SIGNALS:
	void toolConfigChanged(const config::Tools::Tool &tool);
	void profileConfigChanged(const config::Profiles::Profile &profile);
};

}

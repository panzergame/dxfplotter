#pragma once

#include <model/task.h>
#include <config/config.h>

namespace model
{

class Document : public common::Aggregable<Document>
{
private:
	Task::UPtr m_task;
	const config::Tools::Tool *m_toolConfig;
	const config::Profiles::Profile *m_profileConfig;

public:
	explicit Document(Task::UPtr&& task, const config::Tools::Tool &toolConfig, const config::Profiles::Profile &profileConfig);
	Document() = default;
	explicit Document(const Document &other);

	Document &operator=(Document &&) = default;

	Task& task();
	const Task& task() const;

	const config::Tools::Tool &toolConfig() const;
	const config::Profiles::Profile &profileConfig() const;
	void setToolConfig(const config::Tools::Tool &tool);
	void setProfileConfig(const config::Profiles::Profile &profile);
};

}

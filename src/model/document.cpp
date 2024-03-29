#include <document.h>

namespace model
{

Document::Document(Task::UPtr&& task, const config::Tools::Tool &toolConfig, const config::Profiles::Profile &profileConfig)
	:m_task(std::move(task)),
	m_toolConfig(&toolConfig),
	m_profileConfig(&profileConfig)
{

}

Task &Document::task()
{
	return *m_task;
}

const Task &Document::task() const
{
	return *m_task;
}

const config::Tools::Tool &Document::toolConfig() const
{
	return *m_toolConfig;
}

const config::Profiles::Profile &Document::profileConfig() const
{
	return *m_profileConfig;
}

void Document::setToolConfig(const config::Tools::Tool &tool)
{
	m_toolConfig = &tool;
	emit toolConfigChanged(tool);
}

void Document::setProfileConfig(const config::Profiles::Profile &profile)
{
	m_profileConfig = &profile;
	emit profileConfigChanged(profile);
}

}


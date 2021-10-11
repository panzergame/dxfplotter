#include <document.h>

namespace Model
{

Document::Document(Task::UPtr&& task, const Config::Tools::Tool &toolConfig, const Config::Profiles::Profile &profileConfig)
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

const Config::Tools::Tool &Document::toolConfig() const
{
	return *m_toolConfig;
}

const Config::Profiles::Profile &Document::profileConfig() const
{
	return *m_profileConfig;
}

void Document::setToolConfig(const Config::Tools::Tool &tool)
{
	m_toolConfig = &tool;
	emit toolConfigChanged(tool);
}

void Document::setProfileConfig(const Config::Profiles::Profile &profile)
{
	m_profileConfig = &profile;
	emit profileConfigChanged(profile);
}

}


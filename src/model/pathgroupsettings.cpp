#include <pathgroupsettings.h>

#include <QDebug> // TODO

namespace Model
{

void PathGroupSettings::pathSelected(Path *path)
{
	m_selectedPaths.push_back(path);

	emit selectionChanged(m_selectedPaths.size());
}

void PathGroupSettings::pathDeselected(Path *path)
{
	Path::ListPtr::const_iterator it = std::find(m_selectedPaths.begin(), m_selectedPaths.end(), path);

	assert(it != m_selectedPaths.end());


	m_selectedPaths.erase(it);

	emit selectionChanged(m_selectedPaths.size());
}

PathGroupSettings::PathGroupSettings(const Task *task)
{
	connect(task, &Task::pathSelected, this, &PathGroupSettings::pathSelected);
	connect(task, &Task::pathDeselected, this, &PathGroupSettings::pathDeselected);
}

std::optional<float> PathGroupSettings::feedRate() const
{
	return valueIfAllEqual(&PathSettings::feedRate);
}

void PathGroupSettings::setFeedRate(float feedRate)
{
	setValue(&PathSettings::setFeedRate, feedRate);
}

std::optional<float> PathGroupSettings::intensity() const
{
	return valueIfAllEqual(&PathSettings::intensity);
}

void PathGroupSettings::setIntensity(float intensity)
{
	setValue(&PathSettings::setIntensity, intensity);
}

}

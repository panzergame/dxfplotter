#include <pathgroupsettings.h>

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

PathGroupSettings::PathGroupSettings(const Task &task)
{
	// Register selection/deselection on all paths.
	task.forEachPath([this](Path *path) {
		connect(path, &Path::selected, this, [this, path](){ pathSelected(path); });
		connect(path, &Path::deselected, this, [this, path](){ pathDeselected(path); });
	});
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

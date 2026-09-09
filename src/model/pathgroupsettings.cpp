module;

#include <cassert>
#include <QObject>
#include <QDebug>
#include <QtCore/qtmochelpers.h>

export module model.pathgroupsettings;

import common.function;
import model.path;
import model.pathsettings;
import model.task;
export namespace model
{

class PathGroupSettings : public QObject
{
	Q_OBJECT;

private:
	const model::Task& m_task;

	/** Return value of a path settings property if all path have
	 * the same value for the given property.
	 *
	 * @tparam T The returned value type
	 * @tparam Getter The member function used to acces the poperty.
	 */
	template<typename Getter, typename Return = typename common::MemberFunctionTraits<Getter>::Return>
	std::optional<Return> valueIfAllEqual(Getter&& getter) const
	{
		// Reference value of last path to compare with.
		Return lastValue;
		bool firstValue = true;
		bool allEqual = true;

		m_task.forEachSelectedPath([&lastValue, &firstValue, &allEqual, &getter](const model::Path& path) {
			const Return& value = (path.settings().*getter)();
			if (!firstValue && lastValue != value) {
				allEqual = false;
			}
			firstValue = false;
			lastValue = value;
		});

		if (allEqual) {
			return std::make_optional(lastValue);
		}
		return std::nullopt;
	}

	template<typename Setter, typename T>
	void setValue(Setter&& setter, T value)
	{
		m_task.forEachSelectedPath([value, &setter](model::Path& path) {
			(path.settings().*(std::forward<Setter>(setter)))(value);
		});
	}

public:
	explicit PathGroupSettings(const Task& task)
		: m_task(task)
	{
	}

	std::optional<float> planeFeedRate() const { return valueIfAllEqual(&PathSettings::planeFeedRate); }

	void setPlaneFeedRate(float planeFeedRate) { setValue(&PathSettings::setPlaneFeedRate, planeFeedRate); }

	std::optional<float> depthFeedRate() const { return valueIfAllEqual(&PathSettings::depthFeedRate); }

	void setDepthFeedRate(float depthFeedRate) { setValue(&PathSettings::setDepthFeedRate, depthFeedRate); }

	std::optional<float> intensity() const { return valueIfAllEqual(&PathSettings::intensity); }

	void setIntensity(float intensity) { setValue(&PathSettings::setIntensity, intensity); }

	std::optional<float> depth() const { return valueIfAllEqual(&PathSettings::depth); }

	void setDepth(float depth) { setValue(&PathSettings::setDepth, depth); }
};

}

#include "pathgroupsettings.moc"

#pragma once

#include <model/task.h>
#include <model/path.h>
#include <model/pathsettings.h>

#include <cassert>

#include <common/function.h>

namespace Model
{

class PathGroupSettings : public QObject
{
	Q_OBJECT;

private:
	const Model::Task *m_task;

	/** Return value of a path settings property if all path have
	 * the same value for the given property.
	 * 
	 * @tparam T The returned value type
	 * @tparam Getter The member function used to acces the poperty.
	 */
	template <typename Getter, typename Return = typename Common::MemberFunctionTraits<Getter>::Return>
	std::optional<Return> valueIfAllEqual(Getter &&getter) const
	{
		// Reference value of last path to compare with.
		Return lastValue;
		bool allEqual = true;

		m_task->forEachSelectedPath([&lastValue, &allEqual, &getter](Model::Path *path){
			const Return &value = (path->settings().*getter)();
			if (lastValue != value) {
				allEqual = false;
			}
			lastValue = value;
		});

		if (allEqual) {
			return std::make_optional(lastValue);
		}
		return std::nullopt;
	}

	template <typename Setter, typename T>
	void setValue(Setter &&setter, T value)
	{
		m_task->forEachSelectedPath([value, &setter](Model::Path *path){
			(path->settings().*setter)(value);
		});
	}

public:
	explicit PathGroupSettings(const Task *task);

	std::optional<float> feedRate() const;
	void setFeedRate(float feedRate);

	std::optional<float> intensity() const;
	void setIntensity(float intensity);

	std::optional<float> depth() const;
	void setDepth(float depth);
};

}

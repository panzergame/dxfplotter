module;

#include <string>
#include <cstdint>
#include <serializer/access.h>
#include <QObject>
#include <QtCore/qtmochelpers.h>

export module model.renderable;

import common.aggregable;
import geometry.polyline;
import model.pathsettings;
export namespace model
{

class Renderable : public QObject
{
	Q_OBJECT;

	friend serializer::Access<Renderable>;

private:
	std::string m_name;
	Renderable* m_parent;

	struct
	{
		bool m_selected : 1;
		bool m_visible : 1;
		bool m_globallyVisible : 1;
	};

public:
	explicit Renderable(const std::string& name)
		: m_name(name)
		, m_parent(nullptr)
		, m_selected(false)
		, m_visible(true)
		, m_globallyVisible(true)
	{
	}

	explicit Renderable() = default;
	explicit Renderable(const Renderable& other)
		: QObject()
		, m_name(other.name())
		, m_selected(false)
		, m_visible(other.visible())
		, m_globallyVisible(other.m_globallyVisible)

	{
		setParent(other.m_parent);
	}

	const std::string& name() const { return m_name; }
	Renderable* parent() const { return m_parent; }
	void setParent(Renderable* parent)
	{
		if (m_parent) {
			disconnect(m_parent, &Renderable::globalVisibilityChanged, this, &Renderable::updateGlobalVisibility);
		}

		m_parent = parent;
		updateGlobalVisibility();

		if (m_parent) {
			connect(m_parent, &Renderable::globalVisibilityChanged, this, &Renderable::updateGlobalVisibility);
		}
	}

	bool visible() const { return m_visible; }

	void setVisible(bool visible)
	{
		if (m_visible != visible) {
			m_visible = visible;

			updateGlobalVisibility();
			emit visibilityChanged(m_visible);
		}
	}

	void updateGlobalVisibility()
	{
		const bool newGloballyVisible = visible() && (!m_parent || m_parent->globallyVisible());
		if (m_globallyVisible != newGloballyVisible) {
			m_globallyVisible = newGloballyVisible;

			emit globalVisibilityChanged(m_globallyVisible);
		}
	}

	bool globallyVisible() const { return m_globallyVisible; }

	void toggleVisible() { setVisible(!m_visible); }

	bool selected() const { return m_selected; }

	void setSelected(bool selected)
	{
		if (m_selected != selected) {
			m_selected = selected;

			emit selectedChanged(m_selected);
		}
	}

	void deselect();
	void toggleSelect() { setSelected(!m_selected); }

Q_SIGNALS:
	void selectedChanged(bool selected);
	void visibilityChanged(bool visible);
	void globalVisibilityChanged(bool globallyVisible);
};

}

#include "renderable.moc"

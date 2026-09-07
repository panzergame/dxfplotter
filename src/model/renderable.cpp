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

	struct
	{
		bool m_selected : 1;
		bool m_visible : 1;
	};

public:
	explicit Renderable(const std::string& name);
	explicit Renderable() = default;
	explicit Renderable(const Renderable& other);

	const std::string& name() const;

	bool visible() const;
	void setVisible(bool visible);
	void toggleVisible();

	bool selected() const;
	void setSelected(bool selected);
	void deselect();
	void toggleSelect();

Q_SIGNALS:
	void selectedChanged(bool selected);
	void visibilityChanged(bool visible);
};

}

namespace model
{

Renderable::Renderable(const std::string& name)
	: m_name(name)
	, m_selected(false)
	, m_visible(true)
{
}

Renderable::Renderable(const Renderable& other)
	: QObject()
	, m_name(other.name())
	, m_selected(false)
	, m_visible(other.visible())
{
}

const std::string& Renderable::name() const
{
	return m_name;
}

bool Renderable::visible() const
{
	return m_visible;
}

void Renderable::setVisible(bool visible)
{
	if (m_visible != visible) {
		m_visible = visible;

		emit visibilityChanged(m_visible);
	}
}

void Renderable::toggleVisible()
{
	setVisible(!m_visible);
}

bool Renderable::selected() const
{
	return m_selected;
}

void Renderable::setSelected(bool selected)
{
	if (m_selected != selected) {
		m_selected = selected;

		emit selectedChanged(m_selected);
	}
}

void Renderable::toggleSelect()
{
	setSelected(!m_selected);
}

}

#include "renderable.moc"

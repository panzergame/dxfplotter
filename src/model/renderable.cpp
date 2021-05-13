#include <renderable.h>

#include <geometry/cleaner.h>

namespace Model
{

Renderable::Renderable(const std::string &name)
	:m_name(name),
	m_selected(false),
	m_visible(true)
{
}

const std::string &Renderable::name() const
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


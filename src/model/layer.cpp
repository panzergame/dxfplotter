module;

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iterator>
#include <string>
#include <serializer/access.h>
#include <QObject>
#include <QtCore/qtmochelpers.h>

export module model.layer;

import common.aggregable;
import common.copy;
import model.path;
import model.renderable;
export namespace model
{

class Layer : public Renderable, public common::Aggregable<Layer>
{
	Q_OBJECT;

	friend serializer::Access<Layer>;

private:
	Path::ListUPtr m_children;

	void assignSelfToChildren();

public:
	explicit Layer(const std::string& name, Path::ListUPtr&& children);
	explicit Layer() = default;
	explicit Layer(const Layer& other);

	int childrenCount() const;
	Path& childrenAt(int index);
	const Path& childrenAt(int index) const;
	int childIndexFor(const Path& child) const;

	template<class Functor>
	void forEachChild(Functor&& functor)
	{
		for (Path::UPtr& child : m_children) {
			functor(*child);
		}
	}
};

}

namespace model
{

void Layer::assignSelfToChildren()
{
	for (Path::UPtr& child : m_children) {
		child->setParent(this);
	}
}

Layer::Layer(const std::string& name, Path::ListUPtr&& children)
	: Renderable(name)
	, m_children(std::move(children))
{
	assignSelfToChildren();
}

Layer::Layer(const Layer& other)
	: Renderable(other)
	, m_children(common::deepcopy<Path>(other.m_children))
{
	assignSelfToChildren();
}

int Layer::childrenCount() const
{
	return m_children.size();
}

Path& Layer::childrenAt(int index)
{
	assert(0 <= index && index < childrenCount());
	return *m_children[index];
}

const Path& Layer::childrenAt(int index) const
{
	assert(0 <= index && index < childrenCount());
	return *m_children[index];
}

int Layer::childIndexFor(const Path& child) const
{
	const Path::ListUPtr::const_iterator it
		= std::find_if(m_children.cbegin(), m_children.cend(), [&child](const Path::UPtr& ptr) {
			  return ptr.get() == &child;
		  });

	if (it == m_children.cend()) {
		return -1;
	}

	return std::distance(m_children.cbegin(), it);
}

}

#include "layer.moc"

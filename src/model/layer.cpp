#include <layer.h>

#include <common/copy.h>

namespace model
{

void Layer::assignSelfToChildren()
{
	for (Path::UPtr &child : m_children) {
		child->setLayer(*this);
	}
}

Layer::Layer(const std::string &name, Path::ListUPtr &&children)
	:Renderable(name),
	m_children(std::move(children))
{
	assignSelfToChildren();
}

Layer::Layer(const Layer& other)
	:Renderable(other),
	m_children(common::deepcopy<Path>(other.m_children))
{
	for (Path::UPtr &child : m_children) {
		child->setLayer(*this);
	}
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
	const Path::ListUPtr::const_iterator it = std::find_if(m_children.cbegin(), m_children.cend(), [&child](const Path::UPtr &ptr){ return ptr.get() == &child; });

	if (it == m_children.cend()) {
		return -1;
	}

	return std::distance(m_children.cbegin(), it);
}

}


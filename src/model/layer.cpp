#include <layer.h>

namespace Model
{

Layer::Layer(const std::string &name)
	:Renderable(name)
{

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

void Layer::setChildren(Path::ListUPtr &&children)
{
	m_children = std::move(children);
}

}


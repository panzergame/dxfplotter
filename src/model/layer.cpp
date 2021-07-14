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
	const Path::ListPtr::const_iterator it = std::find(m_children.cbegin(), m_children.cend(), &child);

	if (it == m_children.cend()) {
		return -1;
	}

	return std::distance(m_children.cbegin(), it);
}

void Layer::setChildren(const Path::ListUPtr& children)
{
	m_children.resize(children.size());
	std::transform(children.cbegin(), children.cend(), m_children.begin(),
		[](const Path::UPtr &ptr){ return ptr.get(); });
}

}


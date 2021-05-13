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

void Layer::setChildren(const Path::ListUPtr& children)
{
	m_children.resize(children.size());
	std::transform(children.begin(), children.end(), m_children.begin(),
		[](const Path::UPtr &ptr){ return ptr.get(); });
}

}


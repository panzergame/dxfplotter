#include <layer.h>

namespace Model
{

Layer::Layer(const std::string &name, const Path::ListUPtr& children)
	:Renderable(name),
	m_children(children.size())
{
	std::transform(children.begin(), children.end(), m_children.begin(),
		[](const Path::UPtr &ptr){ return ptr.get(); });
}

}


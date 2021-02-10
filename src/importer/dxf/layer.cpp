#include <layer.h>

namespace Importer::Dxf
{

Layer::Layer(bool plot)
	:m_plot(plot)
{
}

bool Layer::visible() const
{
	return m_plot;
}

}



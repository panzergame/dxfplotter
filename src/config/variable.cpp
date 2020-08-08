#include <unordered_map>

#include <section.h>

namespace Config
{

Variable::Type Variable::type() const
{
	return m_type;
}

}

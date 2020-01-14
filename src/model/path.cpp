#include <path.h>

namespace Model
{

Path::Path(Polyline &&polyline, const std::string &name)
	:m_polyline(polyline),
	m_name(name)
{
}

const Polyline &Path::polyline() const
{
	return m_polyline;
}

const std::string &Path::name() const
{
	return m_name;
}

Paths PathsFromPolylines(Polylines &&polylines)
{
	const int size = polylines.size();
	Paths paths(size);

	for (int i = 0; i < size; ++i) {
		paths[i] = Path(std::move(polylines[i]), "Path " + std::to_string(i));
	}

	return paths;
}

}

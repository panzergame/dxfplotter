#pragma once

#include <model/path.h>

#include <serializer/access.h>

namespace Model
{

class Layer : public Renderable, public Common::Aggregable<Layer>
{
	Q_OBJECT;

	friend Serializer::Access<Layer>;

private:
	Path::ListUPtr m_children;

public:
	explicit Layer(const std::string &name);

	int childrenCount() const;
	Path& childrenAt(int index);
	const Path& childrenAt(int index) const;
	int childIndexFor(const Path& child) const;

	template <class Functor>
	void forEachChild(Functor &&functor)
	{
		for (Path::UPtr &child : m_children) {
			functor(*child);
		}
	}

	void setChildren(Path::ListUPtr &&children);
};

}

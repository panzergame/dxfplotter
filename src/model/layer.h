#pragma once

#include <model/path.h>

#include <serializer/access.h>

namespace model
{

class Layer : public Renderable, public common::Aggregable<Layer>
{
	Q_OBJECT;

	friend serializer::Access<Layer>;

private:
	Path::ListUPtr m_children;

	void assignSelfToChildren();

public:
	explicit Layer(const std::string &name, Path::ListUPtr &&children);
	explicit Layer() = default;
	explicit Layer(const Layer& other);;

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
};

}

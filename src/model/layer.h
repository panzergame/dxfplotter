#pragma once

#include <model/path.h>

namespace Model
{

class Layer : public Renderable, public Common::Aggregable<Layer>
{
	Q_OBJECT;

private:
	Path::ListPtr m_children;

public:
	explicit Layer(const std::string &name);

	int childrenCount() const;
	Path& childrenAt(int index);
	const Path& childrenAt(int index) const;
	void setChildren(const Path::ListUPtr& children);
};

}

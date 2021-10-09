#pragma once

#include <geometry/polyline.h>

#include <common/aggregable.h>

#include <model/pathsettings.h>

#include <string>

#include <QObject>

namespace Model
{

class Renderable : public QObject
{
	Q_OBJECT;

	friend Serializer::Access<Renderable>;

private:
	std::string m_name;

	struct {
		bool m_selected : 1;
		bool m_visible : 1;
	};

public:
	explicit Renderable(const std::string &name);
	explicit Renderable() = default;

	const std::string &name() const;

	bool visible() const;
	void setVisible(bool visible);
	void toggleVisible();

	void setSelected(bool selected);
	void deselect();
	void toggleSelect();

Q_SIGNALS:
	void selectedChanged(bool selected);
	void visibilityChanged(bool visible);
};

}

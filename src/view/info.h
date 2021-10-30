#pragma once

#include <uic/ui_info.h>

#include <QWidget>


namespace view
{

namespace view2d
{

class Viewport;

}

class Info : public QWidget, private Ui::Info
{
private:

public:
	explicit Info(view2d::Viewport *viewport);

protected Q_SLOTS:
	void cursorMoved(const QPointF &position);
};

}

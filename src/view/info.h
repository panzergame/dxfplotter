#pragma once

#include <uic/ui_info.h>

#include <QWidget>

namespace View
{

namespace View2d
{

class Viewport;

}

class Info : public QWidget, private Ui::Info
{
private:

public:
	explicit Info(View2d::Viewport *viewport);

protected Q_SLOTS:
	void cursorMoved(const QPointF &position);
};

}

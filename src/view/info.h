#pragma once

#include <uic/ui_info.h>

#include <QWidget>

namespace View
{

class Viewport;

class Info : public QWidget, private Ui::Info
{
private:

public:
	explicit Info(Viewport *viewport);

protected Q_SLOTS:
	void cursorMoved(const QPointF &position);
};

}

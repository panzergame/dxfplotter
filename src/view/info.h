#pragma once

#include <uic/ui_info.h>

#include <QWidget>
#include <QTimer>

namespace model
{

class Application;

}

namespace view
{

namespace view2d
{

class Viewport;

}

class Info : public QWidget, private Ui::Info
{
private:
	QTimer m_timer;

	void showTimedMessage(const QString &content);

public:
	explicit Info(view2d::Viewport *viewport, model::Application &app);

protected Q_SLOTS:
	void cursorMoved(const QPointF &position);
	void fileSaved(const QString &fileName);
	void hideMessage();
};

}

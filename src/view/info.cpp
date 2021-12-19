#include <info.h>
#include <view2d/viewport.h>
#include <QDebug>

namespace view
{

void Info::showTimedMessage(const QString &content)
{
	message->setText(content);
	stackedWidget->setCurrentWidget(messagePage);

	constexpr int showMessageDelay = 2000;
	m_timer.start(showMessageDelay);
}

Info::Info(view2d::Viewport *viewport, model::Application &app)
{
	setupUi(this);

	connect(viewport, &view2d::Viewport::cursorMoved, this, &Info::cursorMoved);
	connect(&app, &model::Application::fileSaved, this, &Info::fileSaved);
	connect(&m_timer, &QTimer::timeout, this, &Info::hideMessage);
}

void Info::cursorMoved(const QPointF &position)
{
	cursorX->setText(QString::number(position.x()));
	cursorY->setText(QString::number(position.y()));
}

void Info::fileSaved(const QString &fileName)
{
	showTimedMessage(QString("Saved %1").arg(fileName));
}

void Info::hideMessage()
{
	stackedWidget->setCurrentWidget(cursorPage);
}

}

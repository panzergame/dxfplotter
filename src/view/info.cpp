module;

#include <uic/ui_info.h>
#include <QWidget>
#include <QTimer>
#include <QDebug>

export module view.info;
import model.application;
import view.view2d.viewport;

export namespace view
{

class Info : public QWidget, private Ui::Info
{
private:
	QTimer m_timer;

	void showTimedMessage(const QString& content)
	{
		message->setText(content);
		stackedWidget->setCurrentWidget(messagePage);

		constexpr int showMessageDelay = 2000;
		m_timer.start(showMessageDelay);
	}

public:
	explicit Info(const view2d::Viewport& viewport, const model::Application& app)
	{
		setupUi(this);

		connect(&viewport, &view2d::Viewport::cursorMoved, this, &Info::cursorMoved);
		connect(&app, &model::Application::fileSaved, this, &Info::fileSaved);
		connect(&m_timer, &QTimer::timeout, this, &Info::hideMessage);
	}

protected Q_SLOTS:
	void cursorMoved(const QPointF& position)
	{
		cursorX->setText(QString::number(position.x()));
		cursorY->setText(QString::number(position.y()));
	}

	void fileSaved(const QString& fileName) { showTimedMessage(QString("Saved %1").arg(fileName)); }

	void hideMessage() { stackedWidget->setCurrentWidget(cursorPage); }
};

}

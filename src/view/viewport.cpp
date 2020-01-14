#include <viewport.h>
#include <pathitem.h>

#include <QDebug> // TODO

namespace View
{

static const QBrush backgroundBrush(QColor(0, 0, 0));

void Viewport::addPathItems()
{
	const Model::PathsPtr &paths = m_app.task().stack();

	for (Model::Path *path : paths) {
		PathItem *item = new PathItem(path);
		scene()->addItem(item);
	}
}

void Viewport::wheelEvent(QWheelEvent *event)
{
	constexpr float SCALE_STEP = 0.2f;

	const float factor = 1.0f + ((event->delta() > 0) ? SCALE_STEP : -SCALE_STEP);

	scale(factor, factor);

	QGraphicsView::wheelEvent(event);
}

void Viewport::mousePressEvent(QMouseEvent *event)
{
	const QPointF pos = mapToScene(event->pos());

	QGraphicsItem *item = scene()->itemAt(pos, QTransform());
	qInfo() << item;

	QGraphicsView::mousePressEvent(event);
}

Viewport::Viewport(Control::Application &app)
	:QGraphicsView(new QGraphicsScene()),
	m_app(app)
{
	setDragMode(QGraphicsView::ScrollHandDrag);
	setRenderHints(QPainter::Antialiasing);
	setBackgroundBrush(View::backgroundBrush);

	addPathItems();
}

}

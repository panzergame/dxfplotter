#include <viewport.h>
#include <pathitem.h>

#include <QDebug> // TODO

namespace View
{

static const QBrush xAxisBrush(QColor(255, 0, 0));
static const QBrush yAxisBrush(QColor(0, 255, 0));
static const QPen xAxisPen(xAxisBrush, 0);
static const QPen yAxisPen(yAxisBrush, 0);

static const QBrush backgroundBrush(QColor(0, 0, 0));

void Viewport::setupPathItems()
{
	m_app.task().forEachPath(
		[scene = scene()](Model::Path *path) {
			PathItem *item = new PathItem(path);
			scene->addItem(item);
		}
	);
}

void Viewport::setupAxes()
{
	scene()->addLine(0.0f, 0.0f, 100.0f, 0.0f, xAxisPen);
	scene()->addLine(0.0f, 0.0f, 0.0f, 100.0f, yAxisPen);
}

void Viewport::setupHighlights()
{
	setupAxes();
}

void Viewport::wheelEvent(QWheelEvent *event)
{
	constexpr float SCALE_STEP = 0.2f;

	const float factor = 1.0f + ((event->delta() > 0) ? SCALE_STEP : -SCALE_STEP);

	scale(factor, factor);

	event->accept();
}

void Viewport::mousePressEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::MiddleButton) {
		m_lastMousePosition = event->pos();
	}

	QGraphicsView::mousePressEvent(event);
}

void Viewport::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::MiddleButton) {
		const QPoint pos = event->pos();
		const QPointF delta = mapToScene(pos) - mapToScene(m_lastMousePosition);

		// Disable anchor to avoid interferences
		setTransformationAnchor(NoAnchor);

		translate(delta.x(), delta.y());

		// Restore anchor
		setTransformationAnchor(AnchorUnderMouse);

		m_lastMousePosition = pos;
	}

	// Forward event used for anchors
	QGraphicsView::mouseMoveEvent(event);
}

void Viewport::selectionChanged()
{
	/*for (QGraphicsItem *item : scene()->selectedItems()) {
		qInfo() << item;
	}*/
}

Viewport::Viewport(Control::Application &app)
	:QGraphicsView(new QGraphicsScene()),
	m_app(app)
{
	// Almost infinte scene boundaries
	setSceneRect(INT_MIN / 2, INT_MIN / 2, INT_MAX, INT_MAX);

	// Disable dragging support
	setDragMode(NoDrag);

	// Anchor under mouse for zooming
	setTransformationAnchor(AnchorUnderMouse);

	setRenderHints(QPainter::Antialiasing);
	setBackgroundBrush(View::backgroundBrush);

	// Hid scroll bars
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	scale(1.0f, -1.0f);

	connect(scene(), &QGraphicsScene::selectionChanged, this, &Viewport::selectionChanged);

	setupHighlights();

	setupPathItems();
}

}

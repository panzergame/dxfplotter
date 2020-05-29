#include <viewport.h>
#include <pathitem.h>

#include <QDebug> // TODO

namespace View
{

constexpr int rubberBandTolerance = 2;
// Rectangle extend used when clicking with empty rubber band.
constexpr QPoint pointSelectionRectExtend(10, 10);

void Viewport::setupPathItems()
{
	m_task->forEachPath(
		[scene = scene()](Model::Path *path) {
			PathItem *item = new PathItem(path);
			scene->addItem(item);
		}
	);
}

void Viewport::startMovement(const QPoint &mousePos)
{
	m_lastMousePosition = mousePos;
}

void Viewport::updateMovement(const QPoint &mousePos)
{
	const QPointF delta = mapToScene(mousePos) - mapToScene(m_lastMousePosition);

	// Disable anchor to avoid interferences
	setTransformationAnchor(NoAnchor);

	translate(delta.x(), delta.y());

	// Restore anchor
	setTransformationAnchor(AnchorUnderMouse);

	m_lastMousePosition = mousePos;
}

void Viewport::startRubberBand(const QPoint &mousePos)
{
	m_rubberBand.start(mousePos, mapToScene(mousePos));
}

void Viewport::updateRubberBand(const QPoint &mousePos)
{
	m_rubberBand.update(mousePos, mapToScene(mousePos));
}

void Viewport::endRubberBand(const QPoint &mousePos, bool addToSelection)
{
	m_rubberBand.end(mousePos, mapToScene(mousePos));

	// Point selection
	if (m_rubberBand.empty(rubberBandTolerance)) {
		// Fake selection area
		const QRect rect(mousePos - pointSelectionRectExtend, mousePos + pointSelectionRectExtend);

		// Find items in fake selection area
		const QList<QGraphicsItem *> items = QGraphicsView::items(rect);
		if (!items.empty()) {
			// Obtain only the first item.
			QGraphicsItem *item = items.front();

			if (!addToSelection) {
				// Clear all and select one in replacive selection
				scene()->clearSelection();
				item->setSelected(true);
			}
			else {
				// Toggle selection in additive selection
				item->setSelected(!item->isSelected());
			}
		}
	}
	// Area selection
	else {
		// Create a path with selection area
		QPainterPath path;
		path.addRect(m_rubberBand.rect());

		scene()->setSelectionArea(path, addToSelection ? Qt::AddToSelection : Qt::ReplaceSelection);
	}
}

void Viewport::setupModel()
{
	setScene(new QGraphicsScene());

	scene()->addItem(&m_rubberBand);

	setupPathItems();

	// Fit scene in view
	const QRectF sceneRect = scene()->sceneRect();
	fitInView(sceneRect, Qt::KeepAspectRatio);
}

void Viewport::drawOriginAxis(QPainter *painter, const QPointF &dir, float scale, const QPen &pen)
{
	static const QPointF center(0.0f, 0.0f);

	painter->setPen(pen);
	painter->drawLine(center, center + dir * scale);
}

void Viewport::drawOrigin(QPainter *painter, float pixelRatio)
{
	// X axis color
	static const QBrush xBrush(QColor(255, 0, 0));
	static const QPen xPen(xBrush, 0);

	// Y axis color
	static const QBrush yBrush(QColor(0, 255, 0));
	static const QPen yPen(yBrush, 0);

	static const QPointF x(1.0f, 0.0f);
	static const QPointF y(0.0f, 1.0f);

	// Axis size in pixel
	static const int axisSize = 20;

	const float scale = axisSize / pixelRatio;

	// raw each axis
	drawOriginAxis(painter, x, scale, xPen);
	drawOriginAxis(painter, y, scale, yPen);
}

void Viewport::taskChanged()
{
	setupModel();
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
	const QPoint &mousePos = event->pos();

	switch (event->button()) {
		case Qt::MidButton:
		{
			startMovement(mousePos);
			break;
		}
		case Qt::LeftButton:
		{
			startRubberBand(mousePos);
			break;
		}
		default:
		{
			break;
		}
	}

	event->accept();
}

void Viewport::mouseReleaseEvent(QMouseEvent *event)
{
	const QPoint &mousePos = event->pos();

	switch (event->button()) {
		case Qt::LeftButton:
		{
			const bool addToSelection = event->modifiers() & Qt::ControlModifier;
			endRubberBand(mousePos, addToSelection);
			break;
		}
		default:
		{
			break;
		}
	}

	event->accept();
}

void Viewport::mouseMoveEvent(QMouseEvent *event)
{
	const QPoint &mousePos = event->pos();
	const Qt::MouseButtons buttons = event->buttons();

	if (buttons & Qt::MiddleButton) {
		updateMovement(mousePos);
	}
	if (buttons & Qt::LeftButton) {
		updateRubberBand(mousePos);
	}

	// Forward event used for anchors
	QGraphicsView::mouseMoveEvent(event);
}

void Viewport::drawBackground(QPainter *painter, const QRectF &rect)
{
	static const QBrush brush(QColor(0, 0, 0));

	//qInfo() << rect << screenRect << geometry();

	const QRectF screenRect = geometry();

	const float pixelRatio = std::max(screenRect.width() / rect.width(), screenRect.height() / rect.height());
	const int resolution = std::log10(pixelRatio);

// 	qInfo() << resolution << pixelRatio;

	painter->fillRect(rect, brush);

	drawOrigin(painter, pixelRatio);
}

Viewport::Viewport(Model::Application &app)
	:TaskModelObserver(app)
{
	// Disable dragging support
	setDragMode(NoDrag);

	// Anchor under mouse for zooming
	setResizeAnchor(NoAnchor);
	setTransformationAnchor(AnchorUnderMouse);

	setRenderHints(QPainter::Antialiasing);

	// Hide scroll bars
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	scale(1.0f, -1.0f);
}

}

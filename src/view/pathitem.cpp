#include <pathitem.h>
#include <model/arc.h>
#include <QtMath>
#include <QDebug>

namespace View
{

static const QBrush normalBrush(QColor(255, 255, 255));
static const QBrush selectBrush(QColor(255, 0, 255));
static const QPen normalPen(normalBrush, 0.0f);
static const QPen selectPen(selectBrush, 0.0f);

class PaintBulge
{
private:
	QPainterPath &m_painter;

public:
	explicit PaintBulge(QPainterPath &painter)
		:m_painter(painter)
	{
	}

	void operator()(const Model::Bulge &bulge)
	{
		if (bulge.isLine()) {
			m_painter.lineTo(bulge.end().toPointF());
		}
		else {
			const Model::Arc arc = bulge.toArc();

			const QVector2D &center = arc.center();
			const float radius = arc.radius();
			const QVector2D corner = center - QVector2D(radius, radius);
			const float size = radius * 2.0f;

			// Start at the bulge begining.
			m_painter.moveTo(bulge.start().toPointF());

			// Draw arc in a bounding square of the arc diameter.
			m_painter.arcTo(corner.x(), corner.y(), size, size,
							qRadiansToDegrees(-arc.startAngle()),
							// Negate span angle because of Y axis swap
							qRadiansToDegrees(-arc.spanAngle()));
		}
	}
};

QPainterPath PathItem::paintPath()
{
	const Model::Polyline &polyline = m_path->polyline();

	QPainterPath painter(polyline.start().toPointF());

	PaintBulge functor(painter);
	polyline.travelAlong(functor);

	return painter;
}

PathItem::PathItem(Model::Path *path)
	:QGraphicsPathItem(QPainterPath()),
	m_path(path)
{
	setPen(normalPen);

	setPath(paintPath());
}

}

#include <pathitem.h>
#include <geometry/arc.h>
#include <QtMath>
#include <QDebug>

namespace View
{

static const QBrush normalBrush(QColor(255, 255, 255));
static const QBrush selectBrush(QColor(80, 0, 255));
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

	void lineToArcPoint(const QVector2D &center, float radius, float angle)
	{
		const QVector2D relativeNormalizedPoint(std::cos(angle), std::sin(angle));
		const QPointF point = (center + relativeNormalizedPoint * radius).toPointF();
		m_painter.lineTo(point);
	}

	void operator()(const Geometry::Bulge &bulge)
	{
		if (bulge.isLine()) {
			m_painter.lineTo(bulge.end().toPointF());
		}
		else {
			const Geometry::Arc arc = bulge.toArc();

			const float maxError = 0.01; // TODO const

			const float radius = arc.radius();
			const QVector2D &center = arc.center();

			// Calculate the angle step to not exceed allowed error (distance from line to arc).
			const float angleStep = std::fmax(std::acos(1.0f - maxError / radius) * 2.0f, 0.0001); // TODO const

			// Pass by starting point.
			m_painter.lineTo(arc.start().toPointF());

			if (arc.orientation() == Geometry::Orientation::CCW) {
				for (float angle = arc.startAngle() + angleStep, end = arc.endAngle(); angle < end; angle += angleStep) {
					lineToArcPoint(center, radius, angle);
				}
			}
			else {
				for (float angle = arc.startAngle() - angleStep, end = arc.endAngle(); angle > end; angle -= angleStep) {
					lineToArcPoint(center, radius, angle);
				}
			}

			// Pass by ending point.
			m_painter.lineTo(arc.end().toPointF());
		}
	}
};

QPainterPath PathItem::paintPath() const
{
	const Geometry::Polyline &polyline = m_path->polyline();

	QPainterPath painter(polyline.start().toPointF());

	PaintBulge functor(painter);
	polyline.forEachBulge(functor);

	return painter;
}

QPainterPath PathItem::shapePath() const
{
	QPainterPathStroker stroker;
	stroker.setWidth(2);
	stroker.setCapStyle(Qt::RoundCap);
	stroker.setJoinStyle(Qt::RoundJoin);

	return stroker.createStroke(m_paintPath);
}

PathItem::PathItem(Model::Path *path)
	:QGraphicsPathItem(QPainterPath()),
	m_path(path),
	m_paintPath(paintPath()),
	m_shapePath(shapePath())
{
	setPen(normalPen);
	setPath(m_shapePath);

	connect(m_path, &Model::Path::selected, this, &PathItem::selected);
	connect(m_path, &Model::Path::deselected, this, &PathItem::deselected);
}

Model::Path *PathItem::path() const
{
	return m_path;
}

QPainterPath PathItem::shape() const
{
	return m_shapePath;
}

void PathItem::selected()
{
	setPen(selectPen);
}

void PathItem::deselected()
{
	setPen(normalPen);
}


}

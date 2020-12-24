#include <bulgepainter.h>
#include <geometry/arc.h>

namespace View::View2d
{

void BulgePainter::lineToArcPoint(const QVector2D &center, float radius, float angle)
{
	const QVector2D relativeNormalizedPoint(std::cos(angle), std::sin(angle));
	const QPointF point = (center + relativeNormalizedPoint * radius).toPointF();
	m_painter.lineTo(point);
}

BulgePainter::BulgePainter(QPainterPath &painter)
	:m_painter(painter)
{
}

void BulgePainter::operator()(const Geometry::Bulge &bulge)
{
	if (bulge.isLine()) {
		const QVector2D &start = bulge.start();
		const QVector2D &end = bulge.end();
		// Check if the bulge is a point
		if (start == end) {
// 			m_painter.point
		}
		else {
			m_painter.lineTo(end.toPointF());
		}
	}
	else {
		const Geometry::Arc arc = bulge.toArc();

		const float maxError = 0.0001; // TODO const

		const float radius = arc.radius();
		const QVector2D &center = arc.center();

		// Calculate the angle step to not exceed allowed error (distance from line to arc).
		const float angleStep = std::fmax(std::acos(1.0f - maxError) * 2.0f, maxError);

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

}

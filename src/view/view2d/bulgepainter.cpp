#include <bulgepainter.h>
#include <utils.h>

#include <geometry/arc.h>

namespace view::view2d
{

void BulgePainter::lineToArcPoint(const Eigen::Vector2d &center, float radius, float angle)
{
	const Eigen::Vector2d relativeNormalizedPoint(std::cos(angle), std::sin(angle));
	const QPointF point = toPointF(center + relativeNormalizedPoint * radius);
	m_painter.lineTo(point);
}

BulgePainter::BulgePainter(QPainterPath &painter)
	:m_painter(painter)
{
}

void BulgePainter::operator()(const geometry::Bulge &bulge)
{
	if (bulge.isLine()) {
		const Eigen::Vector2d &end = bulge.end();
		m_painter.lineTo(toPointF(end));
	}
	else {
		const geometry::Arc arc = bulge.toArc();

		const float maxError = 0.0001; // TODO const

		const float radius = arc.radius();
		const Eigen::Vector2d &center = arc.center();

		// Calculate the angle step to not exceed allowed error (distance from line to arc).
		const float angleStep = std::fmax(std::acos(1.0f - maxError) * 2.0f, maxError);

		// Pass by starting point.
		m_painter.lineTo(toPointF(arc.start()));

		if (arc.orientation() == geometry::Orientation::CCW) {
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
		m_painter.lineTo(toPointF(arc.end()));
	}
}

}

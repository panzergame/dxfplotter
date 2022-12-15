#include <bulgepainter.h>
#include <geometry/arc.h>

namespace view::view2d
{

BulgePainter::BulgePainter(QPainterPath &painter)
	:m_painter(painter)
{
}

void BulgePainter::operator()(const geometry::Bulge &bulge)
{
	if (bulge.isLine()) {
		const QVector2D &end = bulge.end();
		m_painter.lineTo(end.toPointF());
	}
	else {
		const geometry::Arc arc = bulge.toArc();

		const float maxError = 0.0001; // TODO const

		arc.approximateToLinesVisit(maxError, [this](const QVector2D &point){
			m_painter.lineTo(point.toPointF());
		});
	}
}

}

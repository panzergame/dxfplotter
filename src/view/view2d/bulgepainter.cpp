module;

#include <QPainterPath>
#include <QVector2D>

export module view.view2d.bulgepainter;

import geometry.arc;
import geometry.bulge;
import model.path;

export namespace view::view2d
{

class BulgePainter
{
private:
	QPainterPath& m_painter;

	void lineToArcPoint(const QVector2D& center, float radius, float angle);

public:
	explicit BulgePainter(QPainterPath& painter)
		: m_painter(painter)
	{
	}

	void operator()(const geometry::Bulge& bulge)
	{
		if (bulge.isLine()) {
			const QVector2D& end = bulge.end();
			m_painter.lineTo(end.toPointF());
		} else {
			const geometry::Arc arc = bulge.toArc();

			const float maxError = 0.0001; // TODO const

			arc.approximateToLinesVisit(maxError, [this](const QVector2D& point) {
				m_painter.lineTo(point.toPointF());
			});
		}
	}
};

}

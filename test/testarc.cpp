#include <geometry/utils.h>
#include <geometry/bulge.h>

#include <QDebug>
#include <QtMath>

int main()
{
	const QVector2D s(0, 0);
	const QVector2D e(100, 50);

	Geometry::Bulge b1(s, e, 0.5);
	Geometry::Bulge b2(s, e, -0.5);

	Geometry::Arc arc1 = b1.toArc();
	Geometry::Arc arc2 = b2.toArc();

	qInfo() << "center :" << arc1.center() << "start :" << qRadiansToDegrees(arc1.startAngle()) << "end :" << qRadiansToDegrees(arc1.endAngle()) << arc1.radius();
	qInfo() << "center :" << arc2.center() << "start :" << qRadiansToDegrees(arc2.startAngle()) << "end :" << qRadiansToDegrees(arc2.endAngle()) << arc2.radius();
}

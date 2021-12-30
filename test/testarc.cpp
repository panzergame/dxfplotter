#include <geometry/utils.h>
#include <geometry/bulge.h>

#include <QDebug>
#include <QtMath>

int main()
{
	const Eigen::Vector2d s(0, 0);
	const Eigen::Vector2d e(100, 50);

	geometry::Bulge b1(s, e, 0.5);
	geometry::Bulge b2(s, e, -0.5);

	geometry::Arc arc1 = b1.toArc();
	geometry::Arc arc2 = b2.toArc();

	qInfo() << "center :" << arc1.center() << "start :" << qRadiansToDegrees(arc1.startAngle()) << "end :" << qRadiansToDegrees(arc1.endAngle()) << arc1.radius();
	qInfo() << "center :" << arc2.center() << "start :" << qRadiansToDegrees(arc2.startAngle()) << "end :" << qRadiansToDegrees(arc2.endAngle()) << arc2.radius();
}

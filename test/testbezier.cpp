#include <geometry/utils.h>
#include <geometry/bezier.h>

#include <QDebug>

void print(const geometry::Bezier &b)
{
	qInfo() << ".........................";
	qInfo() << b.point1();
	qInfo() << b.control1();
	qInfo() << b.control2();
	qInfo() << b.point2();
}

int main()
{
	const Eigen::Vector2d sa(0, 0);
	const Eigen::Vector2d ea(50, 50);

	const Eigen::Vector2d sb(100, 0);
	const Eigen::Vector2d eb(50, 50);

	/*const std::optional<QVector2D> intersection = geometry::ForwardLineIntersection(sa, ea, sb, eb);
	if (intersection) {
		const Eigen::Vector2d &in = *intersection;
		qInfo() << in;

		const Eigen::Vector2d incenter = geometry::TriangleIncenter(sa, in, sb);
		qInfo() << incenter;
	}
	else {
		qInfo() << "no intersection";
	}*/

	// CCW bezier
	geometry::Bezier bcw(sa, ea, eb, sb);
	// CW bezier
	geometry::Bezier bccw(sb, eb, ea, sa);

	bcw.toBiarc()->toPolyline();
	qInfo() << "..........................";
	bccw.toBiarc()->toPolyline();


	/*geometry::Bezier::Pair p1 = b.splitHalf();
	geometry::Bezier::Pair p2 = b.split(0.5f);

	print(b);
	print(p1[0]);
	print(p1[1]);
	print(p2[0]);
	print(p2[1]);*/

// 	const Eigen::Vector2d ta = ea - sa;
// 	const Eigen::Vector2d tb = eb - sb;
// 
// 	// Determinant
// 	const float d = ta.x() * tb.y() - ta.y() * tb.x();
// 	qInfo() << "det" << d;
// 
// 	/* Det(u, v) = sin(u ^ v) * |u| * |v|
// 	 * 
// 	 * sin(u ^ v) ^ 2 = Det(u, v) ^ 2 / |u| ^ 2 * |v| ^ 2
// 	 */
// 	
// 	// Squared sinus
// 	const float s2 = d / (ta.lengthSquared() * tb.lengthSquared());
// 
// 	qInfo() << "sin2" << s2;


}

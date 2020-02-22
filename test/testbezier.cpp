#include <geometry/utils.h>
#include <geometry/bezier.h>

#include <QDebug>

void print(const Geometry::Bezier &b)
{
	qInfo() << ".........................";
	qInfo() << b.point1();
	qInfo() << b.control1();
	qInfo() << b.control2();
	qInfo() << b.point2();
}

int main()
{
	const QVector2D sa(0, 0);
	const QVector2D ea(1, 5);

	const QVector2D sb(10, 0);
	const QVector2D eb(9, 5);

	const std::optional<QVector2D> intersection = Geometry::ForwardLineIntersection(sa, ea, sb, eb);
	if (intersection) {
		const QVector2D &in = *intersection;
		qInfo() << in;

		const QVector2D incenter = Geometry::TriangleIncenter(sa, in, sb);
		qInfo() << incenter;
	}
	else {
		qInfo() << "no intersection";
	}

	/*Geometry::Bezier b(sa, ea, eb, sb);

	Geometry::Bezier::Pair p1 = b.splitHalf();
	Geometry::Bezier::Pair p2 = b.split(0.5f);

	print(b);
	print(p1[0]);
	print(p1[1]);
	print(p2[0]);
	print(p2[1]);*/

// 	const QVector2D ta = ea - sa;
// 	const QVector2D tb = eb - sb;
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

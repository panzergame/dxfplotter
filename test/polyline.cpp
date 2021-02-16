#include <gtest/gtest.h>
#include <geometry/polyline.h>

constexpr QVector2D point1(1.2, 3.4);
constexpr QVector2D point2(4.5, 6.7);
constexpr QVector2D point3(7.8, 9.1);
constexpr QVector2D point4(11.0, 12.0);

static const Geometry::Bulge pointbulge(point1, point1, 0.0f);
static const Geometry::Bulge bulge1(point1, point2, 0.0f);
static const Geometry::Bulge bulge1next(point2, point3, 0.0f);
static const Geometry::Bulge bulge1invert(point2, point1, 0.0f);
static const Geometry::Bulge bulge2(point3, point4, 0.0f);


TEST(PolylineTest, WithEndAndStartEqualsAndOneBulgeIsPoint)
{
	const Geometry::Polyline polyline1({pointbulge});
	EXPECT_TRUE(polyline1.isPoint());

	const Geometry::Polyline polyline2({bulge1, bulge1invert});
	EXPECT_FALSE(polyline2.isPoint());
}


TEST(PolylineTest, WithBulgeAndInvertBulgeIsClosed)
{
	const Geometry::Polyline polyline1({bulge1, bulge1invert});
	EXPECT_TRUE(polyline1.isClosed());

	const Geometry::Polyline polyline2({bulge1invert, bulge1});
	EXPECT_TRUE(polyline2.isClosed());

	const Geometry::Polyline polyline3({bulge1});
	EXPECT_FALSE(polyline3.isClosed());

	const Geometry::Polyline polyline4({bulge1, bulge2});
	EXPECT_FALSE(polyline4.isClosed());
}

TEST(PolylineTest, ClosedPolylineHasStartEqualsToEnd)
{
	const Geometry::Polyline polyline1({bulge1, bulge1invert});
	EXPECT_EQ(polyline1.start(), polyline1.end());
}

TEST(PolylineTest, StartEndMatchBulgeStartEnd)
{
	const Geometry::Polyline polyline({bulge1, bulge2});
	EXPECT_EQ(polyline.start(), bulge1.start());
	EXPECT_EQ(polyline.end(), bulge2.end());
}

TEST(PolylineTest, InversePolyline)
{
	const Geometry::Polyline polyline({bulge1, bulge2});
	const Geometry::Polyline polylineInverse(polyline.inverse());

	EXPECT_EQ(polyline.start(), polylineInverse.end());
	EXPECT_EQ(polyline.end(), polylineInverse.start());
}

TEST(PolylineTest, ConcatTwoSingleBulgePolylineEqualsTwoBulgePolyline)
{
	const Geometry::Polyline polyline1({bulge1});
	const Geometry::Polyline polyline2({bulge1next});
	const Geometry::Polyline polyline3({bulge1, bulge1next});
	Geometry::Polyline polyline4;
	polyline4 += polyline1;
	polyline4 += polyline2;

	EXPECT_EQ(polyline3.start(), polyline4.start());
	EXPECT_EQ(polyline3.end(), polyline4.end());
}

TEST(PolylineTest, IterateBulge)
{
	const Geometry::Bulge::List bulges{bulge1, bulge1next};
	Geometry::Bulge::List bulges2(bulges);
	const Geometry::Polyline polyline(std::move(bulges2));

	int i = 0;
	polyline.forEachBulge([&i, &bulges](const Geometry::Bulge& bulge){
		EXPECT_EQ(bulge.start(), bulges[i].start());
		EXPECT_EQ(bulge.end(), bulges[i].end());
		++i;
	});
}

TEST(PolylineTest, ScaleTransformIterateBulgeScaled)
{
	const Geometry::Bulge::List bulges{bulge1, bulge1next};
	Geometry::Bulge::List bulges2(bulges);
	Geometry::Polyline polyline(std::move(bulges2));

	polyline.transformBulge([](Geometry::Bulge& bulge){
		bulge.start() *= 0.1;
		bulge.end() *= 0.1;
	});

	int i = 0;
	polyline.forEachBulge([&i, &bulges](const Geometry::Bulge& bulge){
		EXPECT_FLOAT_EQ(bulge.start().x(), bulges[i].start().x() * 0.1);
		EXPECT_FLOAT_EQ(bulge.start().y(), bulges[i].start().y() * 0.1);
		EXPECT_FLOAT_EQ(bulge.end().x(), bulges[i].end().x() * 0.1);
		EXPECT_FLOAT_EQ(bulge.end().y(), bulges[i].end().y() * 0.1);
		++i;
	});
}

TEST(PolylineTest, TestLinePolylineOffsetedHasMovedStartEnd)
{
	const Geometry::Bulge bulge(QVector2D(0.0f, 3.4f), QVector2D(5.0f, 3.4f), 0.0f);
	const Geometry::Polyline polyline({bulge});

	const float offset = 1.2f;
	const Geometry::Polyline::List offseted(polyline.offsetted(offset));

	ASSERT_EQ(offseted.size(), 1);

	const Geometry::Polyline &offsetedPolyline = offseted.front();
	EXPECT_FLOAT_EQ(offsetedPolyline.start().x(), bulge.start().x());
	EXPECT_FLOAT_EQ(offsetedPolyline.end().x(), bulge.end().x());
	EXPECT_FLOAT_EQ(offsetedPolyline.start().y(), bulge.start().y() + offset);
	EXPECT_FLOAT_EQ(offsetedPolyline.end().y(), bulge.end().y() + offset);
}

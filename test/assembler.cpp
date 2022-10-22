#include <gtest/gtest.h>
#include <geometry/filter/assembler.h>

constexpr QVector2D point1(1.2, 3.4);
constexpr QVector2D point2(4.5, 6.7);
constexpr QVector2D point3(7.8, 9.1);
constexpr QVector2D point4(11.0, 12.0);
constexpr QVector2D point5(3.5, 1.2);

static const geometry::Bulge bulge1(point1, point2, 0.2f);
static const geometry::Bulge bulge2(point2, point3, 1.0f);
static const geometry::Bulge bulge3(point3, point4, -0.5f);
static const geometry::Bulge bulgep1(point5, point5, 0.0f);

TEST(AssemblerTest, ShouldMergeAllPolylines)
{
	geometry::Polyline::List rawPolylines{
		geometry::Polyline({bulge1}),
		geometry::Polyline({bulge3}).inverse(),
		geometry::Polyline({bulge2})
	};

	geometry::filter::Assembler assembler(std::move(rawPolylines), 0.01f);

	const geometry::Polyline::List assembledPolylines(assembler.polylines());
	ASSERT_EQ(1, assembledPolylines.size());

	const geometry::Polyline &polyline = assembledPolylines.front();
	QVector2D lastPoint;
	bool isFirstPoint = true;
	polyline.forEachBulge([&lastPoint, &isFirstPoint](const geometry::Bulge& bulge){
		if (!isFirstPoint) {
			EXPECT_EQ(bulge.start(), lastPoint);
		}
		lastPoint = bulge.end();
	});
}

TEST(AssemblerTest, ShouldMergeAllPolylinesLargeTolerance)
{
	geometry::Polyline::List rawPolylines{
		geometry::Polyline({bulge1}),
		geometry::Polyline({bulge3}).inverse(),
		geometry::Polyline({bulge2})
	};

	std::for_each(rawPolylines.begin(), rawPolylines.end(), [](geometry::Polyline &polyline){
		polyline.transformBulge([](geometry::Bulge &bulge){
			bulge.start()[1] += 0.1f;
			bulge.end()[1] -= 0.1f;
		});
	});

	geometry::filter::Assembler assembler(std::move(rawPolylines), 0.1f);

	const geometry::Polyline::List assembledPolylines(assembler.polylines());
	ASSERT_EQ(1, assembledPolylines.size());

	const geometry::Polyline &polyline = assembledPolylines.front();
	QVector2D lastPoint;
	bool isFirstPoint = true;
	polyline.forEachBulge([&lastPoint, &isFirstPoint](const geometry::Bulge& bulge){
		if (!isFirstPoint) {
			EXPECT_EQ(bulge.start(), lastPoint);
		}
		lastPoint = bulge.end();
	});
}

TEST(AssemblerTest, ShouldMergeAllConnectedPolylinesButPoint)
{
	geometry::Polyline::List rawPolylines{
		geometry::Polyline({bulge1}),
		geometry::Polyline({bulge2}),
		geometry::Polyline({bulgep1})
	};

	geometry::filter::Assembler assembler(std::move(rawPolylines), 0.01f);

	const geometry::Polyline::List assembledPolylines(assembler.polylines());
	EXPECT_EQ(2, assembledPolylines.size());
}

TEST(AssemblerTest, ShouldMergePolylinesResultClosed)
{
	const geometry::Bulge b1(point1, point2, 1.0);
	const geometry::Bulge b2(point2, point1, 1.0);

	geometry::Polyline::List rawPolylines{
		geometry::Polyline({b1}),
		geometry::Polyline({b2})
	};

	geometry::filter::Assembler assembler(std::move(rawPolylines), 0.01f);

	const geometry::Polyline::List assembledPolylines(assembler.polylines());
	ASSERT_EQ(1, assembledPolylines.size());

	const geometry::Polyline &polyline = assembledPolylines.front();
	EXPECT_TRUE(polyline.isClosed());
}

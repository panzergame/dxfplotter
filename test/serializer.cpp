#include <exporterfixture.h>

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>

#include <serializer/bulge.h>

#include <sstream>

TEST(Serializer, shouldSerializeVectorWithNoDataLoose)
{
	const QVector2D point(1, 42);

	std::ostringstream output;

	{
		cereal::JSONOutputArchive archive(output);
		archive(point);
	}

	std::istringstream input;
	input.str(output.str());

	{
		cereal::JSONInputArchive archive(input);
		QVector2D outPoint;
		archive(outPoint);

		EXPECT_EQ(point, outPoint);
	}
}

TEST(Serializer, shouldSerializeBulgeWithNoDataLoose)
{
	const geometry::Bulge bulge(QVector2D(0, 0), QVector2D(1, 1), 0);

	std::ostringstream output;

	{
		cereal::JSONOutputArchive archive(output);
		archive(bulge);
	}

	std::istringstream input;
	input.str(output.str());

	{
		cereal::JSONInputArchive archive(input);
		geometry::Bulge outBulge;
		archive(outBulge);

		EXPECT_EQ(bulge, outBulge);
	}
}


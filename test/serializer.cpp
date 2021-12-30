#include <exporterfixture.h>

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>

#include <serializer/bulge.h>

#include <sstream>

TEST(Serializer, shouldSerializeVectorWithNoDataLoose)
{
	const Eigen::Vector2d point(1, 42);

	std::ostringstream output;

	{
		cereal::JSONOutputArchive archive(output);
		archive(point);
	}

	std::istringstream input;
	input.str(output.str());

	{
		cereal::JSONInputArchive archive(input);
		Eigen::Vector2d outPoint;
		archive(outPoint);

		EXPECT_EQ(point, outPoint);
	}
}

TEST(Serializer, shouldSerializeBulgeWithNoDataLoose)
{
	const geometry::Bulge bulge(Eigen::Vector2d::Zero(), Eigen::Vector2d::Ones(), 0);

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


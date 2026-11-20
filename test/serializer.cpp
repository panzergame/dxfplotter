
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

#include <sstream>

#include <QVector2D>
#include <exporterfixture.h>

import geometry.bulge;
import geometry.polyline;
import geometry.tooloperation.tooloperation;
import geometry.utils;
import model.toolpath;
import serializer.bulge;
import serializer.toolpath;
import serializer.access;

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

TEST(Serializer, shouldSerializeToolPathEntries)
{
	const geometry::Polyline polyline({geometry::Bulge(QVector2D(0, 0), QVector2D(1, 1), 0)});
	const geometry::Polyline entry({geometry::Bulge(QVector2D(-1, -1), QVector2D(0, 0), 0)});
	model::ToolPath toolPath({{polyline, entry}, {polyline, std::nullopt}}, geometry::CuttingDirection::BACKWARD);

	std::ostringstream output;

	{
		cereal::JSONOutputArchive archive(output);
		archive(toolPath);
	}

	std::istringstream input;
	input.str(output.str());

	{
		cereal::JSONInputArchive archive(input);
		model::ToolPath outToolPath;
		archive(outToolPath);

		ASSERT_EQ(outToolPath.polylines().size(), 2);
		EXPECT_EQ(outToolPath.polylines()[0].polyline, polyline);
		EXPECT_EQ(outToolPath.polylines()[0].entry, entry);
		EXPECT_EQ(outToolPath.polylines()[1].polyline, polyline);
		EXPECT_FALSE(outToolPath.polylines()[1].entry);
		EXPECT_EQ(outToolPath.cuttingDirection(), geometry::CuttingDirection::BACKWARD);
	}
}

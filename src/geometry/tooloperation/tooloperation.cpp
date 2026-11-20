module;

#include <optional>
#include <vector>

export module geometry.tooloperation.tooloperation;

import geometry.polyline;
import geometry.utils;

export namespace geometry::tooloperation
{

struct ToolPolyline
{
	using List = std::vector<ToolPolyline>;

	Polyline polyline;
	// Brings the tool into cutting condition before polyline, it is never cut to full depth.
	std::optional<Polyline> entry;

	static List FromPolylines(Polyline::List&& polylines)
	{
		List toolPolylines;
		toolPolylines.reserve(polylines.size());
		for (Polyline& polyline : polylines) {
			toolPolylines.push_back({ std::move(polyline), std::nullopt });
		}

		return toolPolylines;
	}
};

class ToolOperation
{
public:
	struct Result
	{
		ToolPolyline::List polylines;
		CuttingDirection cuttingDirection;
	};

	virtual ~ToolOperation() = default;

	virtual Result generate(const Polyline& polyline) const = 0;
};

}

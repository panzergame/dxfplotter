module;

export module geometry.tooloperation.tooloperation;

import geometry.polyline;
import geometry.utils;

export namespace geometry::tooloperation
{

class ToolOperation
{
public:
	struct Result
	{
		Polyline::List polylines;
		CuttingDirection cuttingDirection;
	};

	virtual ~ToolOperation() = default;

	virtual Result generate(const Polyline& polyline) const = 0;
};

}

#include <sorter.h>

namespace geometry
{

Sorter::Sorter(Polyline::List &&polylines)
	:m_polylines(polylines.size())
{
	struct PolylineLength
	{
		Polyline *polyline;
		float length;

		PolylineLength() = default;

		explicit PolylineLength(Polyline &polyline)
			:polyline(&polyline),
			length(polyline.length())
		{
		}

		bool operator<(const PolylineLength& other) const
		{
			return length < other.length;
		}
	};

	std::vector<PolylineLength> polylinesLength(polylines.size());
	std::transform(polylines.begin(), polylines.end(), polylinesLength.begin(),
		   [](Polyline& polyline){ return PolylineLength(polyline); });

	std::sort(polylinesLength.begin(), polylinesLength.end());

	std::transform(polylinesLength.begin(), polylinesLength.end(), m_polylines.begin(),
		   [](PolylineLength& polylineLength){ return std::move(*polylineLength.polyline); });
}

Polyline::List &&Sorter::polylines()
{
	return std::move(m_polylines);
}

}

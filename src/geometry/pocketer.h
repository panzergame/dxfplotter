#pragma once

#include <geometry/polyline.h>

#include <cavc/polylineoffsetislands.hpp>

namespace geometry
{

class Pocketer
{
private:
	const Polyline& m_border;
	const Polyline::ListCPtr &m_islands;
	const float m_margin;
	const float m_minimumPolylineLength;

	Polyline::List m_polylines;

	static bool isCapable(const Polyline &polyline);
	static cavc::OffsetLoop<double> polylineToLoop(const Polyline& polyline, Orientation expectedOrientation);
	static Polyline loopToPolyline(const cavc::OffsetLoop<double> &loop);
	static Polyline::List loopSetToPolylines(const cavc::OffsetLoopSet<double> &loopSet);
	bool canContinueOffsetting(const cavc::OffsetLoopSet<double> &loopSet);
	void pruneSingularities(std::vector<cavc::OffsetLoop<double>> &loops) const;
	cavc::OffsetLoopSet<double> baseLoopSet() const;

public:
	explicit Pocketer(const Polyline &border, const Polyline::ListCPtr &islands, float margin, float minimumPolylineLength);

	Polyline::List &&polylines();
};

}

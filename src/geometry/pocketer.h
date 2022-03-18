#pragma once

#include <geometry/polyline.h>

#include <cavc/polylineoffsetislands.hpp>

namespace geometry
{

class Pocketer
{
private:
	const Polyline& m_border;
	const Orientation m_borderOrientation;
	const Polyline::ListCPtr &m_islands;
	const float m_margin;
	const float m_minimumPolylineLength;
	cavc::ParallelOffsetIslands<double> m_offseter;

	Polyline::List m_polylines;
	Polyline::List m_ccwPolylines;
	Polyline::List m_cwPolylines;

	static bool isCapable(const Polyline &polyline);
	bool isBorderAndInslandsCapable() const;
	static cavc::OffsetLoop<double> polylineToLoop(const Polyline& polyline, Orientation expectedOrientation);
	static cavc::OffsetLoop<double> polylineToLoop(const Polyline& polyline, bool inverse);
	static Polyline loopToPolyline(const cavc::OffsetLoop<double> &loop);
	static Polyline::List loopsToPolylines(const std::vector<cavc::OffsetLoop<double>> &loops);
	bool canContinueOffsetting(const cavc::OffsetLoopSet<double> &loopSet);
	void pruneSingularities(std::vector<cavc::OffsetLoop<double>> &loops) const;
	cavc::OffsetLoopSet<double> baseLoopSet() const;

	cavc::OffsetLoopSet<double> computeNextLoopSet(const cavc::OffsetLoopSet<double> &loopSet);
	void appendLoopSetPolylines(const cavc::OffsetLoopSet<double> &loopSet);
	void appendPolylines(const Polyline::List &polylines, bool inverse);
	void ensurePolylinesOrientationSameAsBorder();

public:
	explicit Pocketer(const Polyline &border, const Polyline::ListCPtr &islands, float margin, float minimumPolylineLength);

	Orientation borderOrientation() const;
	Polyline::List &&polylines();
};

}

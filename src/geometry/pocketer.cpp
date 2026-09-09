module;

#include <algorithm>
#include <vector>
#include <geometry/cavcutils.h>
#include <cavc/polylineoffsetislands.hpp>

export module geometry.pocketer;

import geometry.polyline;
import geometry.utils;

namespace geometry
{

int maxOffsetStep(const cavc::OffsetLoopSet<double>& loopSet, float margin)
{
	const cavc::Polyline<double>& border = loopSet.ccwLoops.front().polyline;

	const cavc::AABB<double> boundingBox = cavc::getExtents(border);
	const float maxSize = std::max(boundingBox.xMax - boundingBox.xMin, boundingBox.yMax - boundingBox.yMin);

	return maxSize / margin / 2.0f;
}

}

export namespace geometry
{

class Pocketer
{
private:
	const Polyline& m_border;
	const Orientation m_borderOrientation;
	const Polyline::ListCPtr& m_islands;
	const float m_margin;
	const float m_minimumPolylineLength;
	cavc::ParallelOffsetIslands<double> m_offseter;

	Polyline::List m_polylines;
	Polyline::List m_ccwPolylines;
	Polyline::List m_cwPolylines;

	static bool isCapable(const Polyline& polyline) { return !polyline.isPoint() && polyline.isClosed(); }

	bool isBorderAndInslandsCapable() const
	{
		return (isCapable(m_border) && std::all_of(m_islands.begin(), m_islands.end(), [](const Polyline* polyline) {
					return isCapable(*polyline);
				}));
	}

	static cavc::OffsetLoop<double> polylineToLoop(const Polyline& polyline, Orientation expectedOrientation)
	{
		const cavc::Polyline loop = polyline.toCavc(expectedOrientation);
		return { 0, loop, cavc::createApproxSpatialIndex(loop) };
	}

	static cavc::OffsetLoop<double> polylineToLoop(const Polyline& polyline, bool inverse)
	{
		const cavc::Polyline loop = inverse ? polyline.inverse().toCavc() : polyline.toCavc();
		return { 0, loop, cavc::createApproxSpatialIndex(loop) };
	}

	static Polyline loopToPolyline(const cavc::OffsetLoop<double>& loop) { return Polyline(loop.polyline); }

	static Polyline::List loopsToPolylines(const std::vector<cavc::OffsetLoop<double>>& loops)
	{
		Polyline::List polylines(loops.size());
		std::transform(loops.begin(), loops.end(), polylines.begin(), loopToPolyline);

		return polylines;
	}

	static bool canContinueOffsetting(const cavc::OffsetLoopSet<double>& loopSet)
	{
		return !loopSet.ccwLoops.empty() || !loopSet.cwLoops.empty();
	}

	void pruneSingularities(std::vector<cavc::OffsetLoop<double>>& loops) const
	{
		std::for_each(loops.begin(), loops.end(),
					  [minimumPolylineLength = (double)m_minimumPolylineLength](cavc::OffsetLoop<double>& loop) {
						  loop.polyline = cavc::pruneSingularities(loop.polyline, minimumPolylineLength);
						  loop.spatialIndex = cavc::createApproxSpatialIndex(loop.polyline);
					  });
	}

	cavc::OffsetLoopSet<double> baseLoopSet() const
	{
		cavc::OffsetLoopSet<double> loopSet;
		const bool inverseBorder = m_borderOrientation != Orientation::CCW;
		loopSet.ccwLoops.push_back(polylineToLoop(m_border, inverseBorder));

		std::transform(m_islands.begin(), m_islands.end(), std::back_inserter(loopSet.cwLoops),
					   [](const Polyline* island) {
						   return polylineToLoop(*island, Orientation::CW);
					   });

		return loopSet;
	}

	cavc::OffsetLoopSet<double> computeNextLoopSet(const cavc::OffsetLoopSet<double>& loopSet)
	{
		cavc::OffsetLoopSet<double> newLoopSet = m_offseter.compute(loopSet, m_margin);

		pruneSingularities(newLoopSet.cwLoops);
		pruneSingularities(newLoopSet.ccwLoops);

		return newLoopSet;
	}

	void appendLoopSetPolylines(const cavc::OffsetLoopSet<double>& loopSet)
	{
		const Polyline::List currentStepOffsettedCcwPolylines = loopsToPolylines(loopSet.ccwLoops);
		const Polyline::List currentStepOffsettedCwPolylines = loopsToPolylines(loopSet.cwLoops);

		m_ccwPolylines.insert(m_ccwPolylines.end(), currentStepOffsettedCcwPolylines.begin(),
							  currentStepOffsettedCcwPolylines.end());
		m_cwPolylines.insert(m_cwPolylines.end(), currentStepOffsettedCwPolylines.begin(),
							 currentStepOffsettedCwPolylines.end());
	}

	void appendPolylines(const Polyline::List& polylines, bool inverse)
	{
		if (inverse) {
			std::transform(polylines.begin(), polylines.end(), std::back_inserter(m_polylines),
						   [](const Polyline& polyline) {
							   return polyline.inverse();
						   });
		} else {
			m_polylines.insert(m_polylines.end(), std::make_move_iterator(polylines.begin()),
							   std::make_move_iterator(polylines.end()));
		}
	}

	void ensurePolylinesOrientationSameAsBorder()
	{
		const bool inverseCcw = (m_borderOrientation != Orientation::CCW);
		appendPolylines(m_ccwPolylines, inverseCcw);
		appendPolylines(m_cwPolylines, !inverseCcw);
	}

public:
	explicit Pocketer(const Polyline& border, const Polyline::ListCPtr& islands, float margin,
					  float minimumPolylineLength)
		: m_border(border)
		, m_borderOrientation(m_border.orientation())
		, m_islands(islands)
		, m_margin(margin)
		, m_minimumPolylineLength(minimumPolylineLength)
	{
		if (isBorderAndInslandsCapable()) {
			cavc::OffsetLoopSet<double> loopSet = baseLoopSet();
			const int maxIteration = maxOffsetStep(loopSet, margin);

			for (int i = 0; i < maxIteration && canContinueOffsetting(loopSet); ++i) {
				loopSet = computeNextLoopSet(loopSet);
				appendLoopSetPolylines(loopSet);
			}

			ensurePolylinesOrientationSameAsBorder();
		}
	}

	Orientation borderOrientation() const { return m_borderOrientation; }

	Polyline::List&& polylines() { return std::move(m_polylines); }
};

}

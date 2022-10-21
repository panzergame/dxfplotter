#include <pocketer.h>

namespace geometry
{

bool Pocketer::isCapable(const Polyline &polyline)
{
	return !polyline.isPoint() && polyline.isClosed();
}

bool Pocketer::isBorderAndInslandsCapable() const
{
	return (isCapable(m_border) && std::all_of(m_islands.begin(), m_islands.end(), [](const Polyline *polyline){ return isCapable(*polyline); }));
}

cavc::OffsetLoop<double> Pocketer::polylineToLoop(const Polyline& polyline, Orientation expectedOrientation)
{
	const cavc::Polyline loop = polyline.toCavc(expectedOrientation);
	return {0, loop, cavc::createApproxSpatialIndex(loop)};
}

cavc::OffsetLoop<double> Pocketer::polylineToLoop(const Polyline& polyline, bool inverse)
{
	const cavc::Polyline loop = inverse ? polyline.inverse().toCavc() :  polyline.toCavc();
	return {0, loop, cavc::createApproxSpatialIndex(loop)};
}

Polyline Pocketer::loopToPolyline(const cavc::OffsetLoop<double> &loop)
{
	return Polyline(loop.polyline);
}

Polyline::List Pocketer::loopsToPolylines(const std::vector<cavc::OffsetLoop<double>> &loops)
{
	Polyline::List polylines(loops.size());
	std::transform(loops.begin(), loops.end(), polylines.begin(), loopToPolyline);

	return polylines;
}

bool Pocketer::canContinueOffsetting(const cavc::OffsetLoopSet<double> &loopSet)
{
	return !loopSet.ccwLoops.empty() || !loopSet.cwLoops.empty();
}

void Pocketer::pruneSingularities(std::vector<cavc::OffsetLoop<double>> &loops) const
{
	std::for_each(loops.begin(), loops.end(), [minimumPolylineLength=(double)m_minimumPolylineLength](cavc::OffsetLoop<double> &loop){
		loop.polyline = cavc::pruneSingularities(loop.polyline, minimumPolylineLength);
		loop.spatialIndex = cavc::createApproxSpatialIndex(loop.polyline);
	});
}

cavc::OffsetLoopSet<double> Pocketer::baseLoopSet() const
{
	cavc::OffsetLoopSet<double> loopSet;
	const bool inverseBorder = m_borderOrientation != Orientation::CCW;
	loopSet.ccwLoops.push_back(polylineToLoop(m_border, inverseBorder));

	std::transform(m_islands.begin(), m_islands.end(), std::back_inserter(loopSet.cwLoops), [](const Polyline *island){
		return polylineToLoop(*island, Orientation::CW);
	});

	return loopSet;
}

int maxOffsetStep(const cavc::OffsetLoopSet<double> &loopSet, float margin)
{
	const cavc::Polyline<double> &border = loopSet.ccwLoops.front().polyline;

	const cavc::AABB<double> boundingBox = cavc::getExtents(border);
	const float maxSize = std::max(boundingBox.xMax - boundingBox.xMin, boundingBox.yMax - boundingBox.yMin);

	return maxSize / margin / 2.0f;
}

cavc::OffsetLoopSet<double> Pocketer::computeNextLoopSet(const cavc::OffsetLoopSet<double> &loopSet)
{
	cavc::OffsetLoopSet<double> newLoopSet = m_offseter.compute(loopSet, m_margin);

	pruneSingularities(newLoopSet.cwLoops);
	pruneSingularities(newLoopSet.ccwLoops);

	return newLoopSet;
}

void Pocketer::appendLoopSetPolylines(const cavc::OffsetLoopSet<double> &loopSet)
{
	const Polyline::List currentStepOffsettedCcwPolylines = loopsToPolylines(loopSet.ccwLoops);
	const Polyline::List currentStepOffsettedCwPolylines = loopsToPolylines(loopSet.cwLoops);

	m_ccwPolylines.insert(m_ccwPolylines.end(), currentStepOffsettedCcwPolylines.begin(), currentStepOffsettedCcwPolylines.end());
	m_cwPolylines.insert(m_cwPolylines.end(), currentStepOffsettedCwPolylines.begin(), currentStepOffsettedCwPolylines.end());
}

void Pocketer::appendPolylines(const Polyline::List &polylines, bool inverse)
{
	if (inverse) {
		std::transform(polylines.begin(), polylines.end(), std::back_inserter(m_polylines),
				[](const Polyline &polyline){ return polyline.inverse(); });
	}
	else {
		m_polylines.insert(m_polylines.end(), std::make_move_iterator(polylines.begin()),
				std::make_move_iterator(polylines.end()));
	}
}

void Pocketer::ensurePolylinesOrientationSameAsBorder()
{
	const bool inverseCcw = (m_borderOrientation != Orientation::CCW);
	appendPolylines(m_ccwPolylines, inverseCcw);
	appendPolylines(m_cwPolylines, !inverseCcw);
}

Pocketer::Pocketer(const Polyline &border, const Polyline::ListCPtr &islands, float margin, float minimumPolylineLength)
	:m_border(border),
	m_borderOrientation(m_border.orientation()),
	m_islands(islands),
	m_margin(margin),
	m_minimumPolylineLength(minimumPolylineLength)
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

Orientation Pocketer::borderOrientation() const
{
	return m_borderOrientation;
}

Polyline::List &&Pocketer::polylines()
{
	return std::move(m_polylines);
}

}

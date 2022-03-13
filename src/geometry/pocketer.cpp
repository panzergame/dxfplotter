#include <pocketer.h>

namespace geometry
{

bool Pocketer::isCapable(const Polyline &polyline)
{
	return !polyline.isPoint() && polyline.isClosed();
}

cavc::OffsetLoop<double> Pocketer::polylineToLoop(const Polyline& polyline, Orientation expectedOrientation)
{
	const cavc::Polyline loop = polyline.toCavc(expectedOrientation);
	return {0, loop, cavc::createApproxSpatialIndex(loop)};
}

Polyline Pocketer::loopToPolyline(const cavc::OffsetLoop<double> &loop)
{
	return Polyline(loop.polyline);
}

Polyline::List Pocketer::loopSetToPolylines(const cavc::OffsetLoopSet<double> &loopSet)
{
	const std::vector<cavc::OffsetLoop<double>> &ccwLoops = loopSet.ccwLoops;
	const std::vector<cavc::OffsetLoop<double>> &cwLoops = loopSet.cwLoops;

	Polyline::List polylines(ccwLoops.size() + cwLoops.size());

	const auto it = std::transform(ccwLoops.begin(), ccwLoops.end(), polylines.begin(), loopToPolyline);
	std::transform(cwLoops.begin(), cwLoops.end(), it, loopToPolyline);

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
	});
}

cavc::OffsetLoopSet<double> Pocketer::baseLoopSet() const
{
	cavc::OffsetLoopSet<double> loopSet;
	loopSet.ccwLoops.push_back(polylineToLoop(m_border, Orientation::CCW));

	std::transform(m_islands.begin(), m_islands.end(), std::back_inserter(loopSet.cwLoops), [](const Polyline *island){
		return polylineToLoop(*island, Orientation::CW);
	});

	return loopSet;
}

int maxOffsetStep(const cavc::Polyline<double>& border, float margin)
{
	const cavc::AABB<double> boundingBox = cavc::getExtents(border);
	const float maxSize = std::max(boundingBox.xMax - boundingBox.xMin, boundingBox.yMax - boundingBox.yMin);

	return maxSize / margin / 2.0f;
}

Pocketer::Pocketer(const Polyline &border, const Polyline::ListCPtr &islands, float margin, float minimumPolylineLength)
	:m_border(border),
	m_islands(islands),
	m_margin(margin),
	m_minimumPolylineLength(minimumPolylineLength)
{
	if (isCapable(border) && std::all_of(islands.begin(), islands.end(), [](const Polyline *polyline){ return isCapable(*polyline); })) {
		cavc::ParallelOffsetIslands<double> alg;
		cavc::OffsetLoopSet<double> loopSet = baseLoopSet();

		const cavc::Polyline<double> cvBorder = loopSet.ccwLoops.front().polyline;
		const int maxIteration = maxOffsetStep(cvBorder, margin);
		for (int i = 0; i < maxIteration && canContinueOffsetting(loopSet); ++i) {
			loopSet = alg.compute(loopSet, m_margin);

			pruneSingularities(loopSet.cwLoops);
			pruneSingularities(loopSet.ccwLoops);

			const Polyline::List currentStepOffsettedPolylines = loopSetToPolylines(loopSet);
			m_polylines.insert(m_polylines.end(), currentStepOffsettedPolylines.begin(), currentStepOffsettedPolylines.end());
		}
	}
}

Polyline::List &&Pocketer::polylines()
{
	return std::move(m_polylines);
}

}

module;

// cavc/staticspatialindex.hpp uses std::uint32_t without including <cstdint>
#include <cstdint>
#include <cavc/mathutils.hpp>

// Must precede every other cavc include so that template instantiations see them.
namespace cavc::utils
{

// absolute threshold to be used for comparing reals generally
template<>
constexpr double realThreshold()
{
	return 1e-12;
}

// absolute threshold to be used for reals in common geometric computation (e.g. to check for
// singularities)
template<>
constexpr double realPrecision()
{
	return 1e-9;
}

// absolute threshold to be used for joining slices together at end points
template<>
constexpr double sliceJoinThreshold()
{
	return 1e-8;
}

// absolute threshold to be used for pruning invalid slices for offset
template<>
constexpr double offsetThreshold()
{
	return 1e-8;
}

}

#include <cavc/plinesegment.hpp>
#include <cavc/polylineoffset.hpp>
#include <cavc/polylineoffsetislands.hpp>

export module geometry.cavc;

export namespace cavc
{

using ::cavc::AABB;
using ::cavc::OffsetLoop;
using ::cavc::OffsetLoopSet;
using ::cavc::ParallelOffsetIslands;
using ::cavc::PlineVertex;
using ::cavc::Polyline;

using ::cavc::createApproxSpatialIndex;
using ::cavc::getExtents;
using ::cavc::parallelOffset;
using ::cavc::pruneSingularities;

}

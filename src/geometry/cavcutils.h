#pragma once

#include <cavc/mathutils.hpp>

namespace cavc
{

namespace utils
{

// absolute threshold to be used for comparing reals generally
template <> constexpr double realThreshold() { return 1e-12; }

// absolute threshold to be used for reals in common geometric computation (e.g. to check for
// singularities)
template <> constexpr double realPrecision() { return 1e-9; }

// absolute threshold to be used for joining slices together at end points
template <> constexpr double sliceJoinThreshold() { return 1e-8; }

// absolute threshold to be used for pruning invalid slices for offset
template <> constexpr double offsetThreshold() { return 1e-8; }

}

}

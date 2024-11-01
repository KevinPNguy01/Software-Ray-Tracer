#include "range.hpp"

const Range Range::empty = Range();
const Range Range::universe = Range(-std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
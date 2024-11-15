#include "aabb.hpp"

const AABB AABB::empty = AABB(Range::empty, Range::empty, Range::empty);
const AABB AABB::universe = AABB(Range::universe, Range::universe, Range::universe);
//
// Created by exde on 30/12/2020.
//

#include "TrackedObject.h"

#include <utility>

TrackedObject::TrackedObject(std::string filename, std::array<int, 3> HSV_min,
							 std::array<int, 3> HSV_max):
		filename(std::move(filename)), HSV_max(HSV_max), HSV_min(HSV_min) {}

//
// Created by exde on 30/12/2020.
//

#include "TrackedObject.h"

#include <utility>

TrackedObject::TrackedObject(std::string filename, std::array<int, 3> HSV_low,
							 std::array<int, 3> HSV_high):
		m_filename(std::move(filename)), m_HSV_high(HSV_high), m_HSV_low(HSV_low) {}

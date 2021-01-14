//
// Created by exde on 30/12/2020.
//

#include "TrackedObject.h"

TrackedObject::TrackedObject(std::string& file_name, std::array<int, 3> HSV_low, std::array<int, 3> HSV_high):
								m_file_name(file_name), m_HSV_high(HSV_high), m_HSV_low(HSV_low) {}

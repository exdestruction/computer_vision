//
// Created by exde on 30/12/2020.
//

#include "TrackedObject.h"

#include <utility>

TrackedObject::TrackedObject(std::string filename, std::array<int, 3> HSV_min,
							 std::array<int, 3> HSV_max):
		filename(std::move(filename)), HSV_max(HSV_max), HSV_min(HSV_min) {}


std::vector<cv::Point2f> TrackedObject::get_rectangle_corners() {
	rectangle_corners.emplace_back(cv::Point2f(rectangle.x, rectangle.y));
	rectangle_corners.emplace_back(cv::Point2f(rectangle.x + rectangle.width,
											   rectangle.y + rectangle.height));
	return this->rectangle_corners;
}

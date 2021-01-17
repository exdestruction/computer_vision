#ifndef IMAGE_PROCESSING_TRACKEDOBJECT_H
#define IMAGE_PROCESSING_TRACKEDOBJECT_H

#include <iostream>
#include <array>
#include <utility>

#include "opencv2/opencv.hpp"

class TrackedObject
{
public:
	std::string filename;
	std::array<int,3> HSV_min{};
	std::array<int,3> HSV_max{};

public:
	cv::Mat descriptor{};
	cv::Mat image{};
	std::vector<cv::KeyPoint> keypoints{};
	cv::Rect rectangle{};
	std::vector<cv::Point2f> rectangle_corners{};







public:
	TrackedObject() = default;
	TrackedObject(std::string filename, std::array<int,3> HSV_min, std::array<int,3> HSV_max);
	~TrackedObject() = default;

	std::vector<cv::Point2f> get_rectangle_corners();


//	//setters and getters declared in header file, common practice
//	void set_HSV_min(std::array<int,3> HSV_min){ this->m_HSV_low = HSV_min;}
//	void set_HSV_max(std::array<int,3> HSV_max){ this->m_HSV_high = HSV_max;}
//	void set_filename(std::string filename){ this->m_filename = std::move(filename);}
//
//	[[nodiscard]] std::array<int,3> get_HSV_min() const{ return this->m_HSV_low;}
//	[[nodiscard]] std::array<int,3> get_HSV_max() const{ return this->m_HSV_high;}
//	[[nodiscard]] std::string get_filename() const{ return this->m_filename; }



};


#endif //IMAGE_PROCESSING_TRACKEDOBJECT_H

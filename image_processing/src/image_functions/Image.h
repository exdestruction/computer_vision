#pragma once
#include <string>

#include "opencv2/opencv.hpp"

class Image
{
private:
	const std::string name;
	const cv::Mat image;
	std::vector<Image*> derived_images{};

public:
	Image(std::string, cv::Mat);

	//void set_name();

	std::string get_name();

	cv::Mat get_image();

	void add_derived_image(const std::string, cv::Mat);

	std::vector<Image*> get_derived_images();

	~Image();
};

 
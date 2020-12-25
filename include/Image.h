#pragma once
#include "main.h"

class Image
{
private:
	const std::string m_name;
	const cv::Mat m_image;
//	std::vector<Image> derived_images{};

public:
	Image(std::string, cv::Mat);

	//void set_name();

	[[nodiscard]] std::string get_name() const;

	[[nodiscard]] cv::Mat get_image() const;

//	void add_derived_image(const std::string, cv::Mat);

//	std::vector<Image*> get_derived_images();

//	~Image();
};

 
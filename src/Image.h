#pragma once
#include <string>
#include <opencv2/opencv.hpp>

class Image
{
private:
	std::string m_name;
	cv::Mat m_image;
	std::vector<Image> m_derived_images{};

public:
	Image(std::string name, cv::Mat image);
	~Image();

	//void set_name();

	std::string get_name() const;

	cv::Mat get_image() const;

	void add_derived_image(const std::string& name, cv::Mat image);

	const std::vector<Image>& get_derived_images() const;


};

 
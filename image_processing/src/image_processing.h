#pragma once
#include <opencv2/opencv.hpp>

cv::Mat resize_image(cv::Mat image, double zoom_factor);
//static void show_binary(const std::string name, cv::Mat* image, int threshold);
void load_images(std::multimap<const std::string, cv::Mat>& images);
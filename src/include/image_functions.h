#pragma once
#include "../main.h"

cv::Mat resize_image(cv::Mat& image, double zoom_factor);
//static void show_binary(const std::string name, cv::Mat* image, int threshold);
void load_images(std::vector<Image*>& dst);
void show_images(const std::vector<Image*> images);
std::vector<Image*> traverse(const std::vector<Image*> images);
cv::Mat illuminate(cv::Mat src, double k);

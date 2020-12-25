#pragma once
//#include "main.h"
#include "Image.h"

class Image;
cv::Mat resize_image(cv::Mat& image, double zoom_factor);
//static void show_binary(const std::string name, cv::Mat* image, int threshold);
void load_images(const std::string& src_path, std::vector<Image>& dst);
void show_images(const std::vector<Image>& images);
//std::vector<Image*> traverse(const std::vector<Image*> images);
cv::Mat illuminate(cv::Mat& src, double k);

//void write_images(const std::vector<Image*> images);

#pragma once
//#include "main.h"
#include <vector>
#include <filesystem>
#include <fstream>

#include "Image.h"
#include "TrackedObject.h"
#include "trackbars.h"

#include "json.hpp"

class Image;


cv::Mat resize_image(cv::Mat& image, double zoom_factor);
//static void show_binary(const std::string name, cv::Mat* image, int threshold);
void load_images(const std::string& src_path, std::vector<Image>& dst);
void show_images(const std::vector<Image>& images);
//std::vector<Image*> traverse(const std::vector<Image*> data);

cv::Mat illuminate(cv::Mat& src, double k);

std::vector<TrackedObject> create_tracking_objects(const std::string& path);

void track_objects(int source, std::vector<TrackedObject>& objects);
//void write_images(const std::vector<Image*> data);

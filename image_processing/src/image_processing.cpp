#include <iostream>
#include <filesystem>
#include <vector>

#include <opencv2/opencv.hpp>
#include "image_processing.h"

int main()
{
	//loading images from ./images to memory
	std::multimap<const std::string, cv::Mat> images;

	std::string path_to_images = "..//images";
	for (const auto& entry: std::filesystem::directory_iterator(path_to_images))
	{
		std::string image_path{ entry.path().u8string() };				//converting types for appopriate usage of 
		if (image_path == (path_to_images + "\\.gitignore"))			//ignoring .gitignore in /images
		{
			continue;
		}

		cv::Mat image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);

		//safety checking
		if (image.data == 0) {
			std::cout << "File "<< image_path << " is invalid for OpenCV\n";
			continue;
		}
		if (image.rows == 0 || image.cols == 0) 
		{
			std::cout << "Failure in show_image: height or width are identical to zero\n";
			continue;
		}

		image = resize_image(image, 0.4);
		images.insert({ image_path, image });
	}

	//opening all windows
	int x_pos = 600;
	int y_pos = 50;
	for (auto& ptr: images)
	{
		const std::string name = ptr.first;
		cv::Mat image = ptr.second;
		cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
		cv::moveWindow(name, x_pos, y_pos);
		cv::imshow(name, image);
		x_pos += 50;
		y_pos += 50;
	}

	//creating and showing window
	cv::waitKey(0);

	cv::destroyAllWindows();


	return 0;
}

//enables resizing window while opening
cv::Mat resize_image(cv::Mat image, double zoom_factor) {
	cv::Size new_size;
	
	//Checking zoom factor
	if (zoom_factor < 1.e-6)
	{
	  std::cout << "Wrong Zoom-factor: too small -> it is set to 1.0\n";
	  zoom_factor = 1.0;
	} 
	int type = image.type();

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	new_size.height = (int)(image.rows * zoom_factor);
	new_size.width = (int)(image.cols * zoom_factor);
	cv::resize(image, image, new_size);
	return image;
}


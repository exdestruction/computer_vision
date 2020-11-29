#include <iostream>
#include <filesystem>
#include <map>

#include <opencv2/opencv.hpp>


#include "main.h"


int main()
{
	//loading images from ./images to memory
	std::multimap<const std::string, cv::Mat> images;
	load_images(images);
	
	//checking if there is any image
	if (images.size() == 0)
	{
		std::cout << "Images not found\n";
		return 0;
	}

	//make binary images
	for(auto ptr = images.begin(); ptr != images.end(); ptr++ )
	{
		const std::string name = (*ptr).first;
		cv::Mat image_threshold, image_adaptive_threshold;
		cv::threshold((*ptr).second, image_threshold, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
		images.insert({ name + " OTSU threshold", image_threshold });
		ptr++;
		cv::adaptiveThreshold(image_threshold, image_adaptive_threshold, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 5, 0);
		images.insert({ name + " adaptive threshold", image_adaptive_threshold });
		ptr++;
	}

	//opening all windows
	int x_pos = 600;
	int y_pos = 50;
	for (const auto& ptr : images)
	{
		const std::string name = ptr.first;
		const cv::Mat *image = &ptr.second;
		cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
		cv::moveWindow(name, x_pos, y_pos);
		cv::imshow(name, *image);
		x_pos += 50;
		y_pos += 50;
	}



	//waiting for any key to be pressed
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

void load_images(std::multimap<const std::string, cv::Mat>& images)
{


	std::string path_to_images = "..//images";
	for (const auto& entry : std::filesystem::directory_iterator(path_to_images))
	{
		std::string image_path{ entry.path().u8string() };				//converting types for appopriate usage of references
		if (image_path == (path_to_images + "\\.gitignore"))			//ignoring .gitignore in /images
		{
			continue;
		}

		//extracting name from the path
		std::string name{ "" };
		for (auto letter = image_path.rbegin(); letter != image_path.rend(); letter++) 
		{
			if (*letter != '\\')
			{
				name += *letter;
			}
			else
			{
				std::reverse(name.begin(), name.end());
				break;
			}
		}





		cv::Mat image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);

		//safety checking
		if (image.data == 0) {
			std::cout << "File " << name << " is invalid for OpenCV\n";
			continue;
		}
		if (image.rows == 0 || image.cols == 0)
		{
			std::cout << "Failure in show_image: height or width are identical to zero\n";
			continue;
		}
		else
		{
			image = resize_image(image, 0.4);
			images.insert({ name, image });
		}

		if (images.size() == 1)
		{
			break;
		}
	}
}




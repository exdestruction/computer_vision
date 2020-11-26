#include <iostream>
#include <opencv2/opencv.hpp>
#include "image_processing.h"

int main(int argc, char** argv)
{
	 cv::Mat image = cv::imread("..//images//fanta.bmp", cv::IMREAD_GRAYSCALE);

	 //safety checking
	 if (image.data == 0) {
		 std::cout << "Image not found\n" << "Press ENTER to quit\n";
		 std::cin.get();
		 return 1;
	 }
	 if (image.rows == 0 || image.cols == 0) {
		 std::cout << "Failure in show_image: height or width are identical to zero\n";
		 return 2;
	 }

	 //window properties
	 auto name = "Image";

	 auto resized_image = resize_image(image, 0.4);

	 //creating and showing window
	 cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
	 cv::moveWindow(name, 600, 50);
	 cv::imshow(name, resized_image);
	 cv::waitKey(0);
	 cv::destroyWindow(name);
}

//enables resizing window while opening
cv::Mat resize_image(cv::Mat image, double zoom_factor) {
	cv::Mat resized_image = image;
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
	cv::resize(image, resized_image, new_size);
	return resized_image;
}


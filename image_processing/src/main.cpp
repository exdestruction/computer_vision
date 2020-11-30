#include <opencv2/opencv.hpp>
#include "main.h"


int main()
{
	//loading images from ./images to memory
	std::vector<Image*> images;
	load_images(images);
	
	//checking if there is any image
	if (images.size() == 0)
	{
		std::cout << "Images not found\n";
		return 0;
	}

	//make binary images
	for(const auto& image: images)
	{
		const std::string name = image->get_name();
		auto image_matrix = image->get_image();
		cv::Mat image_threshold, image_adaptive_threshold;
		cv::threshold(image_matrix, image_threshold, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
		image->add_derived_image(name + "Otsu Threshold", image_threshold);
		cv::adaptiveThreshold(image_threshold, image_adaptive_threshold, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 5, 0);
		image->add_derived_image(name + " adaptive threshold of Otsu threshold", image_adaptive_threshold);
	}

	//opening all windows
	show_images(images);

	//waiting for any key to be pressed
	cv::waitKey();

	cv::destroyAllWindows();

	// cleaning all memory allocations
	for (auto ptr = images.begin(); ptr!= images.end(); ptr++)
	{
		delete *ptr;
	}

	return 0;
}


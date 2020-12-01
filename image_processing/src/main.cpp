#include <opencv2/opencv.hpp>
#include "main.h"



int main()
{
	//loading images from ./images to memory
	std::vector<Image*> images;
	load_images(images);
	
	//checking if there is any imageq
	if (images.size() == 0)
	{
		std::cout << "Images not found\n";
		return 0;
	}

	//main image processing operations
	for(const auto& image: images)
	{
		const std::string name = image->get_name();
		cv::Mat image_colorful = image->get_image().clone();
		cv::Mat image_threshold, image_adaptive_threshold, image_matrix;

		//Tuning original image
		image_colorful.convertTo(image_colorful, -1, 2.6, -120);
		image->add_derived_image(name + " tuned image", image_colorful);

		//convert to grayscale and tune
		cv::cvtColor(image_colorful, image_matrix, cv::COLOR_RGB2GRAY, CV_8U);
		image_matrix.convertTo(image_matrix, -1, 1.3, -70);
		image->add_derived_image(name + " grayscale image", image_matrix);


		if (image->get_name() == "Coca-Cola.bmp")
		{
			cv::adaptiveThreshold(illuminate(image_matrix, 0.1), image_adaptive_threshold, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 321, 10);
			image->add_derived_image(name + " adaptive threshold", image_adaptive_threshold);
		}
		else if (image->get_name() == "fanta.bmp")
		{
			cv::adaptiveThreshold(illuminate(image_matrix, 0.1), image_adaptive_threshold, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 555, 10);
			image->add_derived_image(name + " adaptive threshold", image_adaptive_threshold);
		}
		else if (image->get_name() == "Orangensaft.bmp")
		{
			cv::adaptiveThreshold(illuminate(image_matrix, 0.8), image_adaptive_threshold, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 555, 10);
			image->add_derived_image(name + " adaptive threshold", image_adaptive_threshold);
		}
		else if (image->get_name() == "redbull.bmp")
		{
			//image_matrix.convertTo(image_matrix, -1, 2, -100);
			cv::adaptiveThreshold(illuminate(image_matrix, 0.8), image_adaptive_threshold, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 555, 10);
			image->add_derived_image(name + " adaptive threshold", image_adaptive_threshold);
		}
		
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


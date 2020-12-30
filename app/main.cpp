#include <opencv2/opencv.hpp>
#include "main.h"



int main()
{
    std::string src_dir = CMAKE_SOURCE_DIR;
    src_dir = src_dir + "/images/";


	//loading images from ./images to memory
	std::vector<Image> images;
	load_images(src_dir,images);

	
	//checking if there is any image
	if (images.empty())
	{
		std::cout << "Images not found\n";
		return -1;
	}

	// main image processing operations
	// here is used access via index, because of storing all
	// other images in the same vector, initialized values limit iterations only through
	// original images, not the added one after
    for(int i = 0, end = images.size(); i < end; i++)
	{
        auto image = images[i];
	    const std::string name = image.get_name();
		cv::Mat image_colorful = image.get_image().clone();
		cv::Mat image_threshold, image_adaptive_threshold, image_matrix, image_canny;

		//Tuning original image
		image_colorful.convertTo(image_colorful, -1, 2.6, -120);

		//adding tuned image
		images.emplace_back(name + " tuned", image_colorful);


		//convert to grayscale and tune
		cv::cvtColor(image_colorful, image_matrix, cv::COLOR_BGR2GRAY, CV_8U);
		cv::GaussianBlur(image_matrix, image_matrix, cv::Size(3, 3),
                   0, 0);
		image_matrix.convertTo(image_matrix, -1, 1.3, -70);


		//make binary image
		double illumination_k = 0.5;
		if (image.get_name() == "fanta.bmp")
		{
            illumination_k = 0.1;
		}
		else if (image.get_name() == "orangensaft.bmp")
		{
            illumination_k = 0.8;
		}

        // applying thresholding operation
        cv::adaptiveThreshold(illuminate(image_matrix, illumination_k),
                              image_adaptive_threshold, 255,
                              cv::ADAPTIVE_THRESH_GAUSSIAN_C,
                              cv::THRESH_BINARY, 555, 15);
        images.emplace_back(name + " adaptive Threshold", image_adaptive_threshold);

		//canny filter applied
		int lower_threshold = 35;
		int upper_threshold = 3 * lower_threshold;
		cv::GaussianBlur(image_matrix, image_canny, cv::Size(3, 3),
                   0, 0);
        cv::Canny(image_canny, image_canny, lower_threshold, upper_threshold,
                  3, true);
		
	}

	//showing all windows
	show_images(images);

	return 0;
}


#include <opencv2/opencv.hpp>
#include "main.h"



int main()
{
    std::string src_dir = CMAKE_SOURCE_DIR;
    src_dir = src_dir + "/images";


	//loading images from ./images to memory
	std::vector<Image> images;
	std::vector<Image> images_to_be_shown;
	load_images(src_dir,images);

	
	//checking if there is any image
	if (images.empty())
	{
		std::cout << "Images not found\n";
		return -1;
	}

//	images_to_be_shown.insert(images_to_be_shown.end(), images.begin(), images.end());

	//main image processing operations
//	for(auto& image : images)
    for(int i = 0, end = images.size(); i < end; i++)
	{
//	    images_to_be_shown.push_back(image);
        auto image = images[i];
	    const std::string name = image.get_name();
		cv::Mat image_colorful = image.get_image().clone();
		cv::Mat image_threshold, image_adaptive_threshold, image_matrix, image_canny;

		//Tuning original image
		image_colorful.convertTo(image_colorful, -1, 2.6, -120)
		//adding tuned image
		images.emplace_back(name + " tuned", image_colorful);


		//convert to grayscale and tune
		cv::cvtColor(image_colorful, image_matrix, cv::COLOR_RGB2GRAY, CV_8U);
		cv::GaussianBlur(image_matrix, image_matrix, cv::Size(3, 3),
                   0, 0);
		image_matrix.convertTo(image_matrix, -1, 1.3, -70);
		//image->add_derived_image(name + " grayscale image", image_matrix);


		//make binary image
		if (image.get_name() == "Coca-Cola.bmp")
		{
			cv::adaptiveThreshold(image_matrix, image_adaptive_threshold, 255,
                         cv::ADAPTIVE_THRESH_GAUSSIAN_C,
                         cv::THRESH_BINARY, 555, 5);
//			image->add_derived_image(name + " adaptive threshold", image_adaptive_threshold);
		}
		else if (image.get_name() == "fanta.bmp")
		{
			//image_matrix.convertTo(image_matrix, -1, 0.7, 20);
			//cv::GaussianBlur(image_matrix, image_matrix, cv::Size(5, 5), 0, 0);
			cv::adaptiveThreshold(illuminate(image_matrix, 0.1),
                         image_adaptive_threshold, 255,
                         cv::ADAPTIVE_THRESH_GAUSSIAN_C,
                         cv::THRESH_BINARY, 555, 10);

//			image->add_derived_image(name + " adaptive threshold", image_adaptive_threshold);
		}
		else if (image.get_name() == "Orangensaft.bmp")
		{
			cv::adaptiveThreshold(illuminate(image_matrix, 0.8),
                         image_adaptive_threshold, 255,
                         cv::ADAPTIVE_THRESH_GAUSSIAN_C,
                         cv::THRESH_BINARY, 555, 10);
//			image->add_derived_image(name + " adaptive threshold",
//                            image_adaptive_threshold);
		}
		else if (image.get_name() == "redbull.bmp")
		{
			image_matrix.convertTo(image_matrix, -1, 2, -100);
			cv::adaptiveThreshold(illuminate(image_matrix, 0.5),
                         image_adaptive_threshold, 255,
                         cv::ADAPTIVE_THRESH_GAUSSIAN_C,
                         cv::THRESH_BINARY, 555, 15);
//			image->add_derived_image(name + " adaptive threshold",
//                            image_adaptive_threshold);
		}

		//canny filter applied
		int lower_threshold = 35;
		int upper_threshold = 3 * lower_threshold;
		cv::GaussianBlur(image_matrix, image_canny, cv::Size(3, 3),
                   0, 0);
        cv::Canny(image_canny, image_canny, lower_threshold, upper_threshold,
                  3, true);
		//cv::GaussianBlur(image_colorful, image_canny, cv::Size(3, 3), 0, 0);
		//cv::cvtColor(image_canny, image_canny, cv::COLOR_RGB2GRAY, CV_8U);
		//cv::cvtColor(image_colorful, image_canny, cv::COLOR_RGB2GRAY, CV_8U);
		//image_canny.convertTo(image_canny, -1, 1.1, 0);

//		image->add_derived_image(name + " canny", image_canny);
		
	}

	//showing all windows
	show_images(images);

//	write_images(images);

	//waiting for any key to be pressed
	 cv::waitKey();
	 cv::destroyAllWindows();

	return 0;
}


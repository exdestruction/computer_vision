#include "main.h"

//enables resizing window while opening
cv::Mat resize_image(cv::Mat& image, double zoom_factor) {
	cv::Size new_size;

	//Checking zoom factor
	if (zoom_factor < 1.e-6)
	{
		std::cout << "Wrong Zoom-factor: too small -> it is set to 1.0\n";
		zoom_factor = 1.0;
	}
	int type = (image).type();

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	new_size.height = (int)(image.rows * zoom_factor);
	new_size.width = (int)(image.cols * zoom_factor);
	cv::resize(image, image, new_size);
	return image;
}

void load_images(std::vector<Image*>& dst)
{

	std::string path_to_images = "..\\images";
	for (const auto& entry : std::filesystem::directory_iterator(path_to_images))
	{
		const std::string image_path{ entry.path().u8string() };				//converting types for appopriate usage of references
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
#ifdef SINGLE_IMAGE
		if (name != IMAGE_NAME)
		{
			continue;
		}
#endif

		cv::Mat image = cv::imread(image_path, cv::IMREAD_COLOR);

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
			Image* image_obj = new Image(name, image);
			dst.push_back(image_obj);
		}
#ifdef SINGLE_IMAGE
		if (dst.size() == 1)
		{
			break;
		}
#endif
	}
}

void show_images(const std::vector<Image*> images)
{
	int x_pos = 50;
	int y_pos = 50;
	std::vector<Image*> images_traversed{ traverse(images) };
	for (const auto& image : images_traversed)
	{
		const std::string name = image->get_name();
		cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
		cv::moveWindow(name, x_pos, y_pos);
		cv::imshow(name, image->get_image());
		x_pos += 100;
		y_pos += 30;
	}
}

std::vector<Image*> traverse(const std::vector<Image*> images)
{
	if (images.empty())
	{
		return {};
	}
	std::vector<Image*> result{};
	for (auto& image : images)
	{
		result.push_back(image);
		auto derived = traverse(image->get_derived_images());
		result.insert(result.end(), derived.begin(), derived.end());
	}
	return result;
}




cv::Mat illuminate(cv::Mat src, double k)
{
	cv::Mat filtered_image = cv::Mat::zeros(src.size(), src.type()); 
	cv::Mat illuminated_image = cv::Mat::zeros(src.size(), src.type());

	uchar brightness = (cv::sum(src)[0]) / ((double)src.rows * (double)src.cols);

	//cv::GaussianBlur(src, filtered_image, cv::Size(5, 5), 0, 0);
	cv::blur(src, filtered_image, cv::Size(50,50));

	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x < src.cols; x++)
		{
			illuminated_image.at<uchar>(y, x) = cv::saturate_cast<uchar>(src.at<uchar>(y, x) + k * (brightness - filtered_image.at<uchar>(y, x)));		
		}
	}
	
	return illuminated_image;
}
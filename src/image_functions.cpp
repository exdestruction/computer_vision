#include "image_functions.h"
//#include "cmake_variables/cmake_variables.h"


//enables resizing window while opening
cv::Mat resize_image(cv::Mat image, double zoom_factor) {
	cv::Size new_size;

	//Checking zoom factor
	if (zoom_factor < 1.e-6)
	{
		std::cout << "Wrong Zoom-factor: too small -> it is set to 1.0\n";
		zoom_factor = 1.0;
	}
	int type = (image).type();

    [[maybe_unused]] uchar depth = type & CV_MAT_DEPTH_MASK;
    [[maybe_unused]] uchar channels = 1 + (type >> CV_CN_SHIFT);

	new_size.height = (int)(image.rows * zoom_factor);
	new_size.width = (int)(image.cols * zoom_factor);
	cv::resize(image, image, new_size);
	return image;
}
void load_images(const std::string& src, std::vector<Image>& dst)
{

	for (const auto& entry : std::filesystem::directory_iterator(src))
	{
        //converting types for appropriate usage of references
		auto image_path{ entry.path().u8string() };
        //ignoring .gitignore in /data
		if (image_path == (src + ".gitignore"))
		{
			continue;
		}

		//extracting name from the path
		std::string name;
		for (auto letter = image_path.rbegin(); letter != image_path.rend(); letter++)
		{
			if (*letter != '/')
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
		if (image.data == nullptr) {
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
			image = resize_image(image, 0.5);
//			auto image_obj = new Image(name, image);
			dst.emplace_back(Image(name, image));
		}
#ifdef SINGLE_IMAGE
		if (dst.size() == 1)
		{
			break;
		}
#endif
	}
}
void show_images(const std::vector<Image>& images)
{
	if(images.empty())
	{
		return;
	}
	const std::string original_name = "Original image";
	cv::namedWindow(original_name, cv::WINDOW_AUTOSIZE);
	cv::moveWindow(original_name,0, 0);

	cv::namedWindow("Processed image", cv::WINDOW_AUTOSIZE);
	cv::moveWindow("Processed image", images[0].get_image().cols + 1, 0);

//	Image* image = nullptr;
	for(int i = 0, j = 0;;)
	{
		auto* image = &images[i];
		const std::string name = image->get_name();

		//show original image
		cv::imshow(original_name, image->get_image());

		auto derived = image->get_derived_images()[j];
		auto derived_size = image->get_derived_images().size();
		cv::imshow("Processed image", derived.get_image());


		int button = cv::waitKey() & 255;
		switch(char(button)) {
			default:
				break;
			case 's':
				if (i < images.size() - 1)
				{
					++i;
					j = 0;
				}
				break;
			case 'w':
				if (i > 0)
				{
					--i;
					j = 0;
				}
				break;
			case 'd':
				if ( j + 1 < derived_size)
				{
					++j;
				}
				break;
			case 'a':
				if (j > 0)
				{
					--j;
				}
				break;
		}
		//if ESC pressed
		if (char(button) == 27)
		{
//			cv::destroyAllWindows();
			break;
		}
	}

}
cv::Mat illuminate(cv::Mat& src, double k)
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
			illuminated_image.at<uchar>(y, x) = cv::saturate_cast<uchar>(src.at<uchar>(y, x) +
			        			k * (brightness - filtered_image.at<uchar>(y, x)));
		}
	}
	
	return illuminated_image;
}

void make_binary_mask(const cv::Mat &input_image, cv::Mat &output_image, const std::string& filename,
					  std::array<int, 3> HSV_MIN = {0, 0, 0},
					  std::array<int, 3> HSV_MAX = {0, 0, 0})
{
//	cv::Mat output_image{};
	//if input_image is grayscale
	if (input_image.channels() == 1)
	{
		cv::threshold(input_image, output_image, 0, 255,
					  cv::THRESH_OTSU);
	}
	else
	{
		cv::inRange(input_image, HSV_MIN, HSV_MAX, output_image);
	}

	//if object in binary input_image is black - inverse the input_image
//	if (output_image.at<uchar>(int(output_image.rows / 2), int(output_image.cols / 2)) == 0)
	if(filename == "orangensaft.bmp")
	{
		output_image = ~output_image;
	}


//	//find enclosing contour with maximal area
//	std::vector<std::vector<cv::Point>> contours;
//	std::vector<cv::Vec4i> hierarchy;
//	cv::findContours(output_image, contours, hierarchy,cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE );
//	//fill inner area white
//	std::vector<double> areas{};
//	int max_area_index{};
//	for(auto& contour: contours)
//	{
//		areas.emplace_back(cv::contourArea(contour));
//	}
//	double max_area{areas[0]};
//	for (int i = 0; i < areas.size(); i++)
//	{
//		if(areas[i] > max_area)
//		{
//			max_area_index = i;
//			max_area = areas[i];
//		}
//	}
//	cv::Mat output_image = cv::Mat::zeros(output_image.size(), CV_8U);
//	cv::fillPoly(output_image, contours[max_area_index], 255);

	//morphological operations
	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7,7),
											 cv::Point(-1,-1));
	//opening operations to eliminate all inner black regions
	cv::dilate(output_image, output_image, element, cv::Point(-1, -1), 5);
	cv::erode(output_image, output_image, element, cv::Point(-1, -1), 5);

	//closing operations to eliminate all outer noise
	cv::erode(output_image, output_image, element);
	cv::dilate(output_image, output_image, element);

}


void preprocess(cv::Mat& image)
{


//		if (name == "fanta.bmp")
//		{
//
//		}
	cv::GaussianBlur(image, image, cv::Size(5, 5),
					 0, 0);

	//Tuning original image
	image.convertTo(image, -1, 2.6, -120);

	//convert colors
	cv::cvtColor(image, image, cv::COLOR_BGR2HSV);
}


std::vector<TrackedObject> create_tracking_objects(const std::string& path)
{
	std::vector<TrackedObject> objects{};

	// loading data from ./data/images to memory
	std::vector<Image> images{};
	load_images(path + "images/", images);

	//checking if there is any image
	if (images.empty()) {
		std::cout << "Images not found\n";
		return objects;
	}

	// main image processing operations
	// here is used access via index, because of storing all
	// other data in the same vector, initialized values limit iterations only through
	// original data, not the added one after



	//enabling debug mode
	bool debug_mode;
	char answer{};
	for(;;)
	{
		std::cout << "Do you want debug mode? [y/n] ";
		std::cin >> answer;
		std::cout << std::endl;
//			std::cin.get(answer);
		debug_mode = answer == 'y';
		if (answer == 'y' || answer == 'n')
		{
			break;
		}
	}

	//initialize windows
	if(debug_mode)
	{
		cv::namedWindow("Original image", cv::WINDOW_AUTOSIZE);
		cv::moveWindow("Original image",0, 0);
		cv::namedWindow("Processed image", cv::WINDOW_AUTOSIZE);
		cv::moveWindow("Processed image", images[0].get_image().cols + 1, 0);
	}

	//iterate over all uploaded images to create object
	for (auto &image: images) {
		std::string name = image.get_name();

		//#######################################################
//		cv::Mat grayscale_image{};
//		cv::cvtColor(image.get_image(), grayscale_image, cv::COLOR_BGR2GRAY);

		//#######################################################


		cv::Mat processed_image = image.get_image().clone();

		//preprocess the image
		preprocess(processed_image);
		image.add_derived_image(name + " preprocessed", processed_image.clone());

		//choosing HSV limits
		std::array<int, 3> HSV_MIN{}, HSV_MAX{};
		//choose HSV limits
		if (!debug_mode)
		{
			//pre-chosen HSV limits
			if(name == "fanta.bmp")
			{
				HSV_MIN = {0, 69, 0};
				HSV_MAX = {255, 255, 255};

			}
			else if (name == "orangensaft.bmp")
			{
				HSV_MIN = {0, 0, 174};
				HSV_MAX = {255, 60, 255};
			}

		}
		//manually choose HSV limits
		else
		{
			//check if there is JSON file with properties
			if (std::filesystem::exists(path + "/data/image_properties.json"))
			{
				//get properties from .json if exist
			}
			else
			{
				//create properties manually
				HSV_MIN = {0, 0, 0};
				HSV_MAX = {255, 255, 255};
				create_trackbars(HSV_MIN, HSV_MAX);
				cv::Mat binary_mask{};
				for (;;)
				{
					cv::inRange(image.get_image(), HSV_MIN, HSV_MAX, binary_mask);
					cv::imshow("Processed image", binary_mask);
					auto key = (char) cv::waitKey(30);
					if (key == 27)
					{
						break;
					}
				}
			}
		}

//		cv::Mat threshold_image = processed_image.clone();
//		cv::imshow("Original image", image.get_image());
		cv::Mat binary_mask{};
		make_binary_mask(processed_image, binary_mask, name, HSV_MIN, HSV_MAX);
		image.add_derived_image(name + " Threshold", binary_mask.clone());

		//update given image with the binary mask

		processed_image = cv::Mat::zeros(image.get_image().size(), image.get_image().type());
		image.get_image().copyTo(processed_image, binary_mask);
//		cv::bitwise_and(image.get_image().clone(), binary_mask, processed_image);

		image.add_derived_image(name + " with the mask", processed_image);

		//update created object to vector
		objects.emplace_back(TrackedObject(name, HSV_MIN, HSV_MAX));
	}


	//show images in debug mode
	if(!debug_mode)
	{
		show_images(images);
	}
	cv::destroyAllWindows();

	return objects;
}


void track_objects(int source, std::vector<TrackedObject>& objects)
{
	cv::namedWindow("Video", cv::WINDOW_AUTOSIZE);
	cv::moveWindow("Video", 0, 0);

	//create video stream
    cv::VideoCapture capture(source);
    if(!capture.isOpened())
	{
    	std::cout << "Cannot open source video" << '\n';
    	return;
	}
    cv::Mat frame{};

    for(;;)
	{
    	//equivalent for capture.read(frame);
    	capture >> frame;

		//safety check
		if(frame.rows==0 || frame.cols==0)
		{
			std::cout << "Cannot open video steam!" << std::endl;
 			break;
		}




		for(auto& object: objects)
		{
			cv::Mat binary_image = frame.clone();
			preprocess(binary_image);
			cv::inRange(binary_image, object.get_HSV_min(), object.get_HSV_max(), binary_image);
//			make_binary_mask(frame, binary_image, object.get_filename(), object.get_HSV_min(), object.get_HSV_max());
			cv::imshow("Binary", binary_image);
			cv::waitKey(30);
		}



		//mirror a frame
		cv::flip(frame, frame, +1);
		cv::imshow("Video", frame);
		int button = cv::waitKey(30) & 255;
		//if ESC pressed
		if (char(button) == 27)
		{
			cv::destroyAllWindows();
			break;
		}
	}
    capture.release();

	//iterating through objects
	//make frame binary and detect object due to properties
	//create bounding box over the object
	//add name of the objects

	// wait for key to close video stream
}

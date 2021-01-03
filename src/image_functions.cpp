#include "image_functions.h"


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
			image = resize_image(image, 0.4);
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
	for(int i, j = 0;;)
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
			cv::destroyAllWindows();
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
			illuminated_image.at<uchar>(y, x) = cv::saturate_cast<uchar>(src.at<uchar>(y, x) + k * (brightness - filtered_image.at<uchar>(y, x)));		
		}
	}
	
	return illuminated_image;
}


std::vector<TrackedObject> create_tracking_objects(const std::string& path) {
	std::vector<TrackedObject> objects{};
	//PSEUDOCODE
	// 1. get images
	// 1.1. check if images are in the folder
	// 1.2. load images in vector of cv::Mat


	// loading data from ./data/images to memory
	std::vector<Image> images;
	load_images(path + "images/", images);

	//checking if there is any image
	if (images.empty()) {
		std::cout << "Images not found\n";
		return objects;
	}

	// 	2. process image to extract labels
	// 		2.2. filter the image
	// 		2.3. adjust contrast
	// 		2.4. apply thresholding operations
	// 			2.4.1. if the is already properties in .json -> use them
	// 			2.4.2. if none -> open up the images and choose them manually
	//				2.4.2.1. Add properties (HSV/RGB threshold values) to .json file

	// 	2.5. optimize binary image
	// 		2.5.1 apply morphological operators to eliminate noise


	// 	2.6. detect letters
	// 		2.6.1 look for contours, that contain no objects within ??
	// 		2.6.1 check if contour has straight lines ???
	// 		2.6.1. check for relative location of the objects to detect words???

	//        		check the larges box of neat objects combined ???


	// 3. pass labels to tesseract to get strings
	// initialize tesseract obj ...

	// 4. assign these strings to "name" property  of the object





	//check if there is JSON file with properties
	if (std::filesystem::exists(path + "HSV_properties.json")) {
		//get properties from .json if exist
	} else {
		//create properties manually
	}



	// main image processing operations
	// here is used access via index, because of storing all
	// other data in the same vector, initialized values limit iterations only through
	// original data, not the added one after
	cv::namedWindow("Original image", cv::WINDOW_AUTOSIZE);
	cv::moveWindow("Original image",0, 0);
	cv::namedWindow("Processed image", cv::WINDOW_AUTOSIZE);
	cv::moveWindow("Processed image", images[0].get_image().cols + 1, 0);

	for (auto &image: images) {
		const std::string name = image.get_name();
		cv::Mat processed_image = image.get_image().clone();

		if (name == "fanta.bmp") {
			cv::GaussianBlur(processed_image, processed_image, cv::Size(3, 3),
							 0, 0);
		}
		image.add_derived_image(name + " blurred", processed_image.clone());



		//Tuning original image
		processed_image.convertTo(processed_image, -1, 2.6, -120);
		image.add_derived_image(name + " tuned", processed_image.clone());

		//convert to HSV
//		cv::cvtColor(image->get_image(), image_ver2, cv::COLOR_BGR2HSV);
//		image->add_derived_image(name + " HSV original", image_ver2);

		//convert to HSV
		cv::cvtColor(processed_image, processed_image, cv::COLOR_BGR2HSV);
		image.add_derived_image(name + " HSV", processed_image.clone());


		std::array<int, 3> HSV_MIN_VALUES = {0, 0, 0},
				HSV_MAX_VALUES = {255, 255, 255};
		create_trackbars(HSV_MIN_VALUES, HSV_MAX_VALUES);

		cv::Mat threshold_image = processed_image.clone();
		cv::imshow("Original image", image.get_image());
		for (;;) {
			cv::inRange(processed_image,
						cv::Scalar(HSV_MIN_VALUES[0], HSV_MIN_VALUES[1], HSV_MIN_VALUES[2]),
						cv::Scalar(HSV_MAX_VALUES[0], HSV_MAX_VALUES[1], HSV_MAX_VALUES[2]),
						threshold_image);


			cv::imshow("Processed image", threshold_image);
			auto key = (char) cv::waitKey(30);
			if (key == 27)
			{
				break;
			}
		}
		image.add_derived_image(name + " Threshold", threshold_image);


	}


	//show images in debug mode
	show_images(images);
	return objects;
}



void track_objects(int source, std::vector<TrackedObject>& objects)
{
	//tracking with color???
	//tracking due to movement???





	//create video stream
//    cv::VideoCapture capture(source);

	//iterating through objects
	//make frame binary and detect object due to properties
	//create bounding box over the object
	//add name of the objects

	// wait for key to close video stream
}

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
void illuminate(cv::Mat &src, cv::Mat &output, double k)
{
	cv::Mat filtered_image = cv::Mat::zeros(src.size(), src.type());
//	cv::Mat illuminated_image = cv::Mat::zeros(src.size(), src.type());

	uchar brightness = (cv::sum(src)[0]) / ((double)src.rows * (double)src.cols);

	//cv::GaussianBlur(src, filtered_image, cv::Size(5, 5), 0, 0);
	cv::blur(src, filtered_image, cv::Size(50,50));

	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x < src.cols; x++)
		{
			output.at<uchar>(y, x) = cv::saturate_cast<uchar>(src.at<uchar>(y, x) +
			        			k * (brightness - filtered_image.at<uchar>(y, x)));
		}
	}

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


void preprocess_image(cv::Mat& image)
{


//		if (name == "fanta.bmp")
//		{
//
//		}
	cv::GaussianBlur(image, image, cv::Size(5, 5),
					 0, 0);

	//Tuning original image
	image.convertTo(image, -1, 2.6, -120);

}

void binary_mask_from_borders(cv::Mat& image)
{
	//#######################################################
	std::vector<std::vector<cv::Point> > contours{};
	std::vector<cv::Vec4i> hierarchy{};
	cv::Mat contour_image = cv::Mat::zeros(image.size(), image.type());

	cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
	cv::adaptiveThreshold(image, image, 255, cv::THRESH_BINARY,
					   cv::ADAPTIVE_THRESH_GAUSSIAN_C, 999, 10);
	cv::findContours(image, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	std::vector<std::vector<cv::Point> >hull(contours.size());
	for( size_t i = 0; i < contours.size(); i++ )
	{
		convexHull( contours[i], hull[i] );
	}
	for( int i = 0; i < contours.size(); i++)
	{
		cv::drawContours(contour_image, contours, (int)i, cv::Scalar(255,0,0));
		cv::drawContours(contour_image, hull, (int)i, cv::Scalar(0,255,0));
	}

}


void bounding_rectangle(cv::Mat& image, cv::Rect& rectangle)
{
	std::vector<std::vector<cv::Point> > contours{};
	cv::findContours( image, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE );

	for(auto & contour : contours)
	{
		rectangle |= cv::boundingRect(contour);
	}


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
//	if(debug_mode)
//	{
//		cv::namedWindow("Original image", cv::WINDOW_AUTOSIZE);
//		cv::moveWindow("Original image",0, 0);
//		cv::namedWindow("Processed image", cv::WINDOW_AUTOSIZE);
//		cv::moveWindow("Processed image", images[0].get_image().cols + 1, 0);
//	}

	//iterate over all uploaded images to create object
	for (auto &image: images)
	{
		TrackedObject object{};
		std::string name = image.get_name();
		object.filename = image.get_name();
		//######################################################################
//		cv::Mat canny_image = image.get_image().clone();
//
//		preprocess_image(canny_image);
//		binary_mask_from_borders(canny_image);
//		image.add_derived_image(name + " canny image", canny_image);
		//######################################################################

		cv::Mat processed_image = image.get_image().clone();

		//preprocess the image
		preprocess_image(processed_image);
		//convert colors
		cv::cvtColor(processed_image, processed_image, cv::COLOR_BGR2HSV);
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

		object.HSV_min = HSV_MIN;
		object.HSV_max = HSV_MAX;

		//create binary mask
		cv::Mat binary_mask{};
		make_binary_mask(processed_image, binary_mask, name, HSV_MIN, HSV_MAX);
		image.add_derived_image(name + " Threshold", binary_mask.clone());

		//drawing rectangle
		cv::Mat rectangle_image = cv::Mat::zeros(binary_mask.size(), CV_8UC3);
		cv::Rect rectangle{};
		bounding_rectangle(binary_mask, rectangle);
		object.rectangle = rectangle;
		object.rectangle_corners = object.get_rectangle_corners();
		cv::rectangle(rectangle_image, rectangle.tl(), rectangle.br(),
					  cv::Scalar(0,0,255), 2 );
		image.add_derived_image(name + " rectangle", rectangle_image.clone());


		//update given image with the binary mask
		processed_image = cv::Mat::zeros(image.get_image().size(), image.get_image().type());
		image.get_image().copyTo(processed_image, binary_mask);
		image.add_derived_image(name + " with the mask", processed_image);

		//detect key points in the image with ORB
		object.image = image.get_image().clone();
		object.keypoints = detect_keypoints(object.image);

		//calculate descriptors (feature vectors)
		drawKeypoints( object.image, object.keypoints, object.image);
		image.add_derived_image(name + " keypoints", object.image.clone());

		object.descriptor = extract_descriptor(object.image, object.keypoints);

		//update created object to vector
		objects.emplace_back(object);
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
	const float ratio_thresh = 0.7f;
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




//		for(auto& object: objects)
//		{
//			cv::Mat binary_image = frame.clone();
//			preprocess_image(binary_image);
//			cv::inRange(binary_image, object.get_HSV_min(), object.get_HSV_max(), binary_image);
////			make_binary_mask(frame, binary_image, object.get_filename(), object.get_HSV_min(), object.get_HSV_max());
//			cv::imshow("Binary", binary_image);
//			cv::waitKey(30);
//		}
		auto keypoints = detect_keypoints(frame);
//		if (keypoints.empty())
//		{
//			continue;
//		}
		auto descriptor = extract_descriptor(frame, keypoints);
//		if(descriptor.empty())
//		{
//			continue;
//		}

		for(auto& object: objects)
		{

			if(object.descriptor.empty() || descriptor.empty())
			{
				continue;
			}
			auto knn_matches = match_descriptors(object.descriptor, descriptor);

			if(knn_matches.empty())
			{
				continue;
			}

			//-- Filter matches using the Lowe's ratio test
			std::vector<cv::DMatch> good_matches{};
			for(auto & knn_match : knn_matches)
			{
				if (knn_match[0].distance < ratio_thresh * knn_match[1].distance)
				{
					good_matches.emplace_back(knn_match[0]);
				}
			}

			std::vector<cv::Point2f> obj{};
			std::vector<cv::Point2f> scene{};
//			cv::Rect rectangle{};
			std::vector<cv::Point2f> rectangle_corners(object.rectangle_corners.size());
			for(auto& match: good_matches)
			{
				obj.emplace_back(object.keypoints[match.queryIdx].pt);
				scene.emplace_back(keypoints[match.trainIdx].pt);
			}
			if(scene.empty())
			{
				continue;
			}
			if (obj.size() > 4)
			{
				cv::Mat homography = cv::findHomography(obj, scene, cv::RANSAC);
				if(homography.empty())
				{
					continue;
				}
				cv::perspectiveTransform(object.rectangle_corners,
							 rectangle_corners, homography);
				cv::rectangle(frame, rectangle_corners[0], rectangle_corners[1],
				  cv::Scalar(0,0,255), 3);
				cv::putText(frame, object.filename, cv::Point(rectangle_corners[0].x, rectangle_corners[0].y - 10),
				cv::FONT_HERSHEY_PLAIN, 1,
				cv::Scalar(0,255,0));

			}

//			cv::Mat img_matches{};
//			cv::drawMatches( object.image, object.keypoints, frame, keypoints,
//				good_matches, img_matches, cv::Scalar::all(-1),
//				cv::Scalar::all(-1),
//				std::vector<char>(),
//				cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS  );
//			//-- Show detected matches
//			cv::imshow("Good Matches", img_matches);
//			cv::waitKey(30);
		}



		//mirror a frame
//		cv::flip(frame, frame, +1);

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
}

#include "trackbars.h"

[[maybe_unused]] void on_trackbar(int, void*)
{

}

void create_trackbars(std::array<int, 3>& HSV_MIN_VALUES, std::array<int, 3>& HSV_MAX_VALUES)
{
	const std::string trackbar_name = "Trackbars";
	cv::namedWindow(trackbar_name, 0);
	cv::moveWindow(trackbar_name, 0,0 );
	//create memory to store trackbar name on window
//	char TrackbarName[50];
//	sprintf( TrackbarName, "H_MIN", H_MIN);
//	sprintf( TrackbarName, "H_MAX", H_MAX);
//	sprintf( TrackbarName, "S_MIN", S_MIN);
//	sprintf( TrackbarName, "S_MAX", S_MAX);
//	sprintf( TrackbarName, "V_MIN", V_MIN);
//	sprintf( TrackbarName, "V_MAX", V_MAX);

//	int H_MAX = 255;


//	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH),
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->
	cv::createTrackbar( "H_MIN", trackbar_name, &HSV_MIN_VALUES[0],
						255, on_trackbar );
	cv::createTrackbar( "H_MAX", trackbar_name, &HSV_MAX_VALUES[0],
						255, on_trackbar );
	cv::createTrackbar( "S_MIN", trackbar_name, &HSV_MIN_VALUES[1],
						255, on_trackbar );
	cv::createTrackbar( "S_MAX", trackbar_name, &HSV_MAX_VALUES[1],
						255, on_trackbar );
	cv::createTrackbar( "V_MIN", trackbar_name, &HSV_MIN_VALUES[2],
						255, on_trackbar );
	cv::createTrackbar( "V_MAX", trackbar_name, &HSV_MAX_VALUES[2],
						255, on_trackbar );
}

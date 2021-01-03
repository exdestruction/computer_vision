#include <opencv2/opencv.hpp>
#include "main.h"



int main()
{
    //specifying source directory for data to be tracked
    std::string src_dir = CMAKE_SOURCE_DIR;
    src_dir = src_dir + "/data/";

    //create objects from data
    std::vector<TrackedObject> objects = create_tracking_objects(src_dir);

    if (objects.empty())
    {
        return -1;
    }



	//function to track objects on the specified video
	// 0 - source from webcam
	// 1 - source from video in data/video
	track_objects(0, objects);

	return 0;
}


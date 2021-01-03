
#ifndef IMAGE_PROCESSING_TRACKBARS_H
#define IMAGE_PROCESSING_TRACKBARS_H

#include "opencv2/opencv.hpp"


[[maybe_unused]] void on_trackbar(int, void*);

void create_trackbars(std::array<int, 3>& HSV_MIN_VALUES, std::array<int, 3>& HSV_MAX_VALUES);

#endif //IMAGE_PROCESSING_TRACKBARS_H

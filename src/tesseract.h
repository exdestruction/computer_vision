//
// Created by exde on 18/01/2021.
//

#ifndef COMPUTER_VISION_TESSERACT_H
#define COMPUTER_VISION_TESSERACT_H


#include <iostream>

#include "opencv2/opencv.hpp"
#include "tesseract/baseapi.h"
#include "leptonica/allheaders.h"

std::string get_letters(cv::Mat &image);

#endif //COMPUTER_VISION_TESSERACT_H

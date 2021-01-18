//
// Created by exde on 16/01/2021.
//


#ifndef COMPUTER_VISION_ORB_FUNCTIONS_H
#define COMPUTER_VISION_ORB_FUNCTIONS_H

#include "opencv2/opencv.hpp"

std::vector<cv::KeyPoint> detect_keypoints(cv::Mat& src);
cv::Mat extract_descriptor(cv::Mat& src, std::vector<cv::KeyPoint>& keypoints );
std::vector<std::vector<cv::DMatch>> match_descriptors(cv::Mat& query_descriptor,
													   cv::Mat& train_descriptor);


#endif //COMPUTER_VISION_ORB_FUNCTIONS_H

//
// Created by exde on 16/01/2021.
//

#include "orb_functions.h"

std::vector<cv::KeyPoint> detect_keypoints(cv::Mat& src)
{
	cv::Ptr<cv::FeatureDetector> detector = cv::ORB::create();
	std::vector<cv::KeyPoint> keypoints{};
	detector->detect( src, keypoints );

	return keypoints;
}

cv::Mat extract_descriptor(cv::Mat& src, std::vector<cv::KeyPoint>& keypoints )
{
	cv::Mat descriptor{};
	cv::Ptr<cv::DescriptorExtractor> extractor = cv::ORB::create();
	extractor->compute(src, keypoints, descriptor);


	return descriptor;
}

std::vector<std::vector<cv::DMatch>> match_descriptors(cv::Mat& query_descriptor,
													   cv::Mat& train_descriptor)
{
	cv::Ptr<cv::DescriptorMatcher> matcher =
//			cv::DescriptorMatcher::create(cv::DescriptorMatcher::BRUTEFORCE);
			cv::DescriptorMatcher::create(cv::DescriptorMatcher::BRUTEFORCE_HAMMING);
	std::vector<std::vector<cv::DMatch>> knn_matches{};
	matcher->knnMatch(query_descriptor, train_descriptor, knn_matches, 2 );
	return knn_matches;
}

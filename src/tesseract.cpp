//
// Created by exde on 18/01/2021.
//

#include "tesseract.h"

std::string get_letters(cv::Mat &image)
{
	auto *api = new tesseract::TessBaseAPI();
//	tesseract::TessBaseAPI api;
//	if(api->Init(NULL, "end"))
//	api->Init(nullptr, "eng", tesseract::OEM_LSTM_ONLY);
	if (api->Init(nullptr, "eng")) {
		std::cout << "Could not initialize TesseractAPI!" << std::endl;
		return {};
	}
	api->SetPageSegMode(tesseract::PSM_AUTO);
	api->SetVariable("debug_file", "tesseract.log");
//
	api->SetImage(image.data, image.cols, image.rows, 3, image.step);
//
	std::string result = api->GetUTF8Text();
//
	api->End();
	delete api;
	return result;
}

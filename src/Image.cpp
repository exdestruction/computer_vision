#include "Image.h"

Image::Image(std::string name, cv::Mat image) : m_name(name), m_image(image) {}

//Image::~Image()
//{
//	if (!this->derived_images.empty())
//	{
//		//deleting all derived images
//        for (auto & derived_image : this->derived_images)
//        {
//
//            delete derived_image;
//        }
//	}
//}

std::string Image::get_name() const
{
	return this->m_name;
}

cv::Mat Image::get_image() const
{
//	cv::Mat image = this->image;
	return this->m_image;
}

//void Image::add_derived_image(std::string name, cv::Mat image)
//{
////	auto derived_image = new Image(name, image);
//	this->derived_images.push_back(new Image(name, image));
//}

//std::vector<Image*> Image::get_derived_images()
//{
//	return this->derived_images;
//}

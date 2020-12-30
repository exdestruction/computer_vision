#include "Image.h"

#include <utility>

Image::Image(std::string name, cv::Mat image) : m_name(std::move(name)), m_image(std::move(image)) {}

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

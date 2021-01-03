#include "Image.h"

#include <utility>


Image::Image(std::string name, cv::Mat image):
		m_name(std::move(name)),
		m_image(std::move(image)) {}

Image::~Image() = default;

//Image::~Image()
//{
//	if (!this->m_derived_images.empty())
//	{
//		//deleting all derived data
//        for (auto & derived_image : this->m_derived_images)
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

void Image::add_derived_image(const std::string& name, cv::Mat image)
{
	//	auto derived_image = new Image(name, image);
	bool unique = true;
	if (!m_derived_images.empty())
	{
		for (auto& derived_image: m_derived_images)
		{
			if (name == derived_image.get_name())
			{
				unique = false;
			}
		}
	}
	if (unique)
	{
		this->m_derived_images.emplace_back(Image(name, std::move(image)));
	}
}

const std::vector<Image>& Image::get_derived_images() const
{
	return this->m_derived_images;
}

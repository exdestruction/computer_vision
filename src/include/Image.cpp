#include "Image.h"

Image::Image(std::string name, cv::Mat image) : name(name), image(image) {};

Image::~Image()
{
	if (!this->derived_images.empty())
	{
		//deleting all derived images
		for (auto ptr = this->derived_images.begin(); ptr != this->derived_images.end(); ptr++)
		{
			delete (*ptr);
		}
	}
}

std::string Image::get_name()
{
	return this->name;
}

cv::Mat Image::get_image()
{
	cv::Mat image = this->image;
	return image;
}

void Image::add_derived_image(const std::string name, cv::Mat image)
{
	Image* derived_image = new Image(name, image);
	this->derived_images.push_back(derived_image);
}

std::vector<Image*> Image::get_derived_images()
{
	return this->derived_images;
}

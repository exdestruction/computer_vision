#ifndef IMAGE_PROCESSING_TRACKEDOBJECT_H
#define IMAGE_PROCESSING_TRACKEDOBJECT_H

#include <iostream>
#include <array>
#include <utility>

class TrackedObject
{
private:
	std::string m_filename;
	std::array<int,3> m_HSV_low{};
	std::array<int,3> m_HSV_high{};
public:
	TrackedObject() = default;
	TrackedObject(std::string filename, std::array<int,3> HSV_low, std::array<int,3> HSV_high);
	~TrackedObject() = default;

	//setters and getters declared in header file, common practice
	void set_HSV_min(std::array<int,3> HSV_low){ this->m_HSV_low = HSV_low;}
	void set_HSV_max(std::array<int,3> HSV_high){ this->m_HSV_high = HSV_high;}
	void set_filename(std::string filename){ this->m_filename = std::move(filename);}

	[[nodiscard]] std::array<int,3> get_HSV_min() const{ return this->m_HSV_low;}
	[[nodiscard]] std::array<int,3> get_HSV_max() const{ return this->m_HSV_high;}
	[[nodiscard]] std::string get_filename() const{ return this->m_filename; }



};


#endif //IMAGE_PROCESSING_TRACKEDOBJECT_H

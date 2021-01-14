#ifndef IMAGE_PROCESSING_TRACKEDOBJECT_H
#define IMAGE_PROCESSING_TRACKEDOBJECT_H

#include <iostream>
#include <array>

class TrackedObject
{
public:
	std::string m_file_name;
	std::array<int,3> m_HSV_low;
	std::array<int,3> m_HSV_high;
public:
	TrackedObject() = default;
	TrackedObject(std::string &file_name, std::array<int,3> HSV_low, std::array<int,3> HSV_high);
	~TrackedObject() = default;

	//setters and getters declared in header file, common practice
	void set_HSV_min(std::array<int,3>& HSV_low){ this->m_HSV_low = HSV_low;}
	void set_HSV_max(std::array<int,3>& HSV_high){ this->m_HSV_high = HSV_high;}
	std::array<int,3> get_HSV_min(){ return this->m_HSV_low;}
	std::array<int,3> get_HSV_max(){ return this->m_HSV_high;}

};


#endif //IMAGE_PROCESSING_TRACKEDOBJECT_H

project(app CXX)

set(target_name ${CMAKE_PROJECT_NAME})


# OpenCV_DIR could also be read from system environment variable.
if(WIN32)
	set(OpenCV_DIR "d:/libs/opencv-2.4.8/build")
else()
	set(OpenCV_DIR "/opt/opencv/release")
endif()

find_package(OpenCV REQUIRED)
#
#include_directories(${CMAKE_SOURCE_DIR})
#
#include_directories(${OpenCV_INCLUDE_DIRS}) # not needed for opencv>=4.0
#add_subdirectory(include)
set(SOURCE "")

file(GLOB SOURCE ../src/*.cpp ./*.cpp)

add_executable(${target_name} ${SOURCE})

target_link_libraries(${target_name} PRIVATE ${OpenCV_LIBS})

target_include_directories(${target_name} PRIVATE ../include)

target_include_directories(${target_name} PRIVATE ${CMAKE_SOURCE_DIR})

configure_file (
		"${CMAKE_SOURCE_DIR}/include/cmake_variables/cmake_variables.h.in"
		"${CMAKE_SOURCE_DIR}/include/cmake_variables/cmake_variables.h")
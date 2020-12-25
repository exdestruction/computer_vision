project(src CXX)

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
set(SOURCES "")

file(GLOB SOURCES *.cpp)
#file(GLOB SOURCES ../include/*.h)

add_library(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})

target_include_directories(${PROJECT_NAME} PRIVATE include)

#target_link_libraries(${PROJECT_NAME} include)

#target_link_libraries(${PROJECT_NAME} )
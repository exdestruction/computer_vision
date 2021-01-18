project(app CXX)

set(target_name ${CMAKE_PROJECT_NAME})


# OpenCV_DIR could also be read from system environment variable.
if(WIN32)
	set(OpenCV_DIR "d:/libs/opencv-2.4.8/build")
else()
	set(OpenCV_DIR "/opt/opencv/release")
#	set(Tesseract_DIR "/opt/tesseract/build")
#	set(Leptonica_DIR "/usr/local/lib/cmake")
endif()


find_package( PkgConfig REQUIRED)

pkg_search_module( Tesseract REQUIRED tesseract )
pkg_search_module( Leptonica REQUIRED lept )

#find_package(Tesseract REQUIRED)
#find_package(Leptonica REQUIRED)


#looking for packages
find_package(OpenCV REQUIRED)


#include_directories(${Tesseract_INCLUDE_DIRS})
#include_directories(${Leptonica_INCLUDE_DIRS})

#include_directories(/usr/local/include/tesseract)
#include_directories(/usr/local/include/leptonica)


set(SOURCE "")

file(GLOB SOURCE ../src/*.cpp ../src/*.h ../src/*.hpp
		*.cpp ../include/*.h ../include/*.hpp ../vendor/include/*.hpp)

add_executable(${target_name} ${SOURCE})

target_link_libraries(${target_name} PRIVATE ${OpenCV_LIBS})
#target_link_libraries(${target_name} PRIVATE ${Leptonica_LIBS})
#target_link_libraries(${target_name} PRIVATE ${Tesseract_LIBS})
target_link_libraries(${target_name} PRIVATE -ltesseract -llept)


#adding include directories to compiler
# target_include_directories(${target_name} PRIVATE ${Tesseract_INCLUDE_DIRS})
# target_include_directories(${target_name} PRIVATE ${Leptonica_INCLUDE_DIRS})

target_include_directories(${target_name} PRIVATE ${CMAKE_SOURCE_DIR}/include
		${CMAKE_SOURCE_DIR}/vendor/include)

#making cmake variables to interact with the code
configure_file ("${CMAKE_SOURCE_DIR}/include/cmake_variables/cmake_variables.h.in"
				"${CMAKE_SOURCE_DIR}/include/cmake_variables/cmake_variables.h")

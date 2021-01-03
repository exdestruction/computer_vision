project(app CXX)

set(target_name ${CMAKE_PROJECT_NAME})


# OpenCV_DIR could also be read from system environment variable.
if(WIN32)
	set(OpenCV_DIR "d:/libs/opencv-2.4.8/build")
else()
	set(OpenCV_DIR "/opt/opencv/release")
endif()


find_package( PkgConfig REQUIRED)

pkg_search_module( TESSERACT REQUIRED tesseract )

pkg_search_module( LEPTONICA REQUIRED lept )

# link_directories( ${TESSERACT_LIBRARY_DIRS} )

# link_directories( ${LEPTONICA_LIBRARY_DIRS} )

#looking for packages
find_package(OpenCV REQUIRED)


include_directories(${TESSERACT_INCLUDE_DIRS})
include_directories(${Leptonica_INCLUDE_DIRS})


set(SOURCE "")

file(GLOB SOURCE ../src/*.cpp *.cpp ../include/*.h ../include/*.hpp ../vendor/include/*.hpp)

add_executable(${target_name} ${SOURCE})

target_link_libraries(${target_name} PRIVATE ${OpenCV_LIBS})
# target_link_libraries(${target_name} PRIVATE ${Leptonica_LIBRARIES})
target_link_libraries(${target_name} PRIVATE ${TESSERACT_LIBRARIES})
# target_link_libraries(${target_name} PRIVATE libtesseract)


#adding include directories to compiler
# target_include_directories(${target_name} PRIVATE ${Tesseract_INCLUDE_DIRS})
# target_include_directories(${target_name} PRIVATE ${Leptonica_INCLUDE_DIRS})

target_include_directories(${target_name} PRIVATE ${CMAKE_SOURCE_DIR}/include
		${CMAKE_SOURCE_DIR}/vendor/include)

#making cmake variables to interact with the code
configure_file (
		"${CMAKE_SOURCE_DIR}/include/cmake_variables/cmake_variables.h.in"
		"${CMAKE_SOURCE_DIR}/include/cmake_variables/cmake_variables.h")

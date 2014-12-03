project(ImageIn C CXX)
cmake_minimum_required(VERSION 2.8.11)

find_package(JPEG)
if(JPEG_FOUND)
	include_directories(${JPEG_INCLUDE_DIR})
else()
	message(FATAL_ERROR "Cannot find JPEG library. Please add the JPEG library directory to CMAKE_PREFIX_PATH.")
endif()

find_package(PNG)
if(PNG_FOUND)
	include_directories(${PNG_INCLUDE_DIRS})
else()
	message(FATAL_ERROR "Cannot find PNG library. Please add the PNG library directory to CMAKE_PREFIX_PATH.")
endif()

find_package(Threads REQUIRED)

# On Unix platforms, ImageIn library have to be compiled with Positional Independent code flag.
# This ensure the shared libraries built in 'plugins' folder will link with ImageIn
# This flag is useless on Windows (MSVC or MinGW compilers)
if(UNIX)
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -fPIC)
endif()

set(ImageIn_SOURCES
	Algorithm.h
	Algorithm/Average.h
	Algorithm/Binarization.h
	Algorithm/BitPlane.h
	Algorithm/ComponentLabeling.h
	Algorithm/Difference.h
	Algorithm/Distance.h
	Algorithm/Dithering.h
	Algorithm/EdgeColoration.h
	Algorithm/Filter.cpp
	Algorithm/Filter.h
	Algorithm/Filtering.cpp
	Algorithm/Filtering.h
	Algorithm/Identity.h
	Algorithm/Inversion.h
	Algorithm/MorphoMat.cpp
	Algorithm/MorphoMat.h
	Algorithm/Otsu.h
	Algorithm/RgbToGrayscale.h
	AlgorithmCollection.h
	AlgorithmException.h
	Array.h
	BadImageException.h
	BinaryImage.cpp
	BinaryImage.h
	BmpImage.cpp
	BmpImage.h
	Converter.h
	EasyBMP.cpp
	EasyBMP.h
	EasyBMP_BMP.h
	EasyBMP_DataStructures.h
	EasyBMP_VariousBMPutilities.h
	GenericAlgorithm.h
	Graph.cpp
	Graph.h
	GrayscaleImage.h
	Histogram.h
	Image.h
	ImageFile.h
	ImageFileAbsFactory.cpp
	ImageFileAbsFactory.h
	ImageFileException.h
	ImageFileFactory.cpp
	ImageFileFactory.h
	JpgImage.cpp
	JpgImage.h
	mystdint.h
	PixelAlgorithm.h
	PngImage.cpp
	PngImage.h
	ProjectionHistogram.h
	pstdint.h
	Rectangle.h
	RgbImage.h
	SpecificAlgorithm.h
	UnknownFormatException.h
	VffImage.cpp
	VffImage.h
)
set(CXX_TEMPLATES
	SpecificAlgorithm.tpp
	RgbImage.tpp
	ProjectionHistogram.tpp
	Image.tpp
	Histogram.tpp
	GrayscaleImage.tpp
	GenericAlgorithm.tpp
	Converter.tpp
	Algorithm/RgbToGrayscale.tpp
	Algorithm/Otsu.tpp
	Algorithm.tpp
	Algorithm/Binarization.tpp
	Algorithm/ComponentLabeling.tpp
	Algorithm/Dithering.tpp
)

if(NOT WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

add_library(ImageIn OBJECT ${ImageIn_SOURCES})

add_executable(ImageIn_main main.cpp $<TARGET_OBJECTS:ImageIn>)
target_link_libraries(ImageIn_main ${JPEG_LIBRARY} ${PNG_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

add_executable(ImageIn_test test_image.cpp $<TARGET_OBJECTS:ImageIn>)
target_link_libraries(ImageIn_test ${JPEG_LIBRARY} ${PNG_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
# Needed to execute the tester
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test)

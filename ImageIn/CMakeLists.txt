project(ImageIn C CXX)
cmake_minimum_required(VERSION 2.8.11)

find_library(JPEG_LIB NAMES jpeg)
find_library(PNG_LIB NAMES png)

if(NOT (JPEG_LIB AND PNG_LIB))
	message(FATAL_ERROR "JPEG and PNG librarys hasn't been found. You have to install them before running the compilation of this software")
endif()

find_package(Threads REQUIRED)

set(ImageIn_SOURCES
	Algorithm.h
	Algorithm.tpp
	Algorithm/Average.h
	Algorithm/Binarization.h
	Algorithm/Binarization.tpp
	Algorithm/BitPlane.h
	Algorithm/ComponentLabeling.h
	Algorithm/ComponentLabeling.tpp
	Algorithm/Difference.h
	Algorithm/Distance.h
	Algorithm/Dithering.h
	Algorithm/Dithering.tpp
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
	Algorithm/Otsu.tpp
	Algorithm/RgbToGrayscale.h
	Algorithm/RgbToGrayscale.tpp
	AlgorithmCollection.h
	AlgorithmException.h
	Array.h
	BadImageException.h
	BinaryImage.cpp
	BinaryImage.h
	BmpImage.cpp
	BmpImage.h
	Converter.h
	Converter.tpp
	EasyBMP.cpp
	EasyBMP.h
	EasyBMP_BMP.h
	EasyBMP_DataStructures.h
	EasyBMP_VariousBMPutilities.h
	GenericAlgorithm.h
	GenericAlgorithm.tpp
	Graph.cpp
	Graph.h
	GrayscaleImage.h
	GrayscaleImage.tpp
	Histogram.h
	Histogram.tpp
	Image.h
	Image.tpp
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
	ProjectionHistogram.tpp
	pstdint.h
	Rectangle.h
	RgbImage.h
	RgbImage.tpp
	SpecificAlgorithm.h
	SpecificAlgorithm.tpp
	UnknownFormatException.h
	VffImage.cpp
	VffImage.h
)

add_library(ImageIn STATIC ${ImageIn_SOURCES})
target_link_libraries(ImageIn ${JPEG_LIB} ${PNG_LIB} ${CMAKE_THREAD_LIBS_INIT})

add_executable(ImageIn_main main.cpp)
target_link_libraries(ImageIn_main ImageIn ${JPEG_LIB} ${PNG_LIB} ${CMAKE_THREAD_LIBS_INIT})

add_executable(ImageIn_test test_image.cpp)
target_link_libraries(ImageIn_test ImageIn ${JPEG_LIB} ${PNG_LIB} ${CMAKE_THREAD_LIBS_INIT})
# Needed to execute the tester
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test)

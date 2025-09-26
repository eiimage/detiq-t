cmake_minimum_required(VERSION 3.16)
project(ImageIn_Test LANGUAGES C CXX)
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

# Qt6 (tests)
find_package(Qt6Core REQUIRED)
find_package(Qt6Widgets REQUIRED)
find_package(Qt6Test REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
enable_testing()

# On Unix platforms, ImageIn library have to be compiled with Positional Independent code flag.
# This ensure the shared libraries built in 'plugins' folder will link with ImageIn
# This flag is useless on Windows (MSVC or MinGW compilers)
if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

set(detiq-t_TEST_SOURCES
	AlgorithmTest.h  
	BasicApplicationTest.cpp
	BinarizationTester.h
	BitPlaneTest.h
	ComponentLabelingTest.h 
	ComponentLabelingTester.h 
	ConvertedTest.h
	ConverterTest.h  
	ConverterTester.h                    
	CopyTest.h   
	CoreTester.h  
	CropTest.h         
	FilteringTest.h
	FilteringTester.h
	GenericInterfaceTest.cpp
	HistogramTest.h 
    	ImageDiff.h
	IOTest.h
	IOTester.h
	MorphoMatTester.h
	ProjHistTest.h
	Test.h        
	Tester.h          
	UtilityTester.h
)

if(NOT WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

include_directories(${CMAKE_CURRENT_LIST_DIR}/../GenericInterface)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../ImageIn)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../ImageIn/Algorithm)

add_executable(detiq-t_Test main.cpp  $<TARGET_OBJECTS:GenericInterface> $<TARGET_OBJECTS:ImageIn>)

target_link_libraries(detiq-t_Test
		  Qt6::Core
		  Qt6::Widgets
		  Qt6::Test
		  # ↓ Ces libs Qt6 supplémentaires couvrent les objets provenant de GenericInterface/ImageIn +  Qt6::Svg
		  Qt6::PrintSupport
		  Qt6::OpenGL
		  Qt6::OpenGLWidgets
		  ${QWT_LIBRARY}       # Qwt (Qt6)
		  ${QWT_LIBRARIES}     # si votre FindQwt définit aussi QWT_LIBRARIES, on garde
		  ${JPEG_LIBRARY}
		  ${PNG_LIBRARIES}
		  Threads::Threads
		)
project(GenericInterface C CXX)
cmake_minimum_required(VERSION 2.8.11)

include_directories(../ImageIn)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(Qwt 6.0 REQUIRED)
if(QWT_FOUND)
	include_directories(${QWT_INCLUDE_DIRS})
else()
	message(FATAL_ERROR "Cannot find Qwt library. Please add the Qwt library directory to CMAKE_PREFIX_PATH.")
endif()

# Find the Qt libraries
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

include_directories(
	${Qt5Core_INCLUDE_DIRS}
	${Qt5Widgets_INCLUDE_DIRS}
	${Qt5Gui_INCLUDE_DIRS}
)

set(GenInterface_SOURCES
	Exceptions.h
	GenericInterface.cpp
	GenericInterface.h
	InterfaceExceptions.h
	Service.h
	ServiceExceptions.h
	Services/AlgorithmService.cpp
	Services/AlgorithmService.h
	Services/FileService.cpp
	Services/FileService.h
	Services/Node.h
	Services/UtilityService.cpp
	Services/UtilityService.h
	Services/WindowService.cpp
	Services/WindowService.h
	Utilities/Log.cpp
	Utilities/Log.h
	Widgets/ImageListBox.h
	Widgets/AboutDialog.h
	Widgets/AboutDialog.cpp
	Widgets/ImageWidgets/AlternativeImageView.cpp
	Widgets/ImageWidgets/AlternativeImageView.h
	Widgets/ImageWidgets/DoubleImageWindow.cpp
	Widgets/ImageWidgets/DoubleImageWindow.h
	Widgets/ImageWidgets/GenericHistogramView.cpp
	Widgets/ImageWidgets/GenericHistogramView.h
	Widgets/ImageWidgets/GenericHistogramWindow.cpp
	Widgets/ImageWidgets/GenericHistogramWindow.h
	Widgets/ImageWidgets/GraphicalHistogram.cpp
	Widgets/ImageWidgets/GraphicalHistogram.h
	Widgets/ImageWidgets/GridView.cpp
	Widgets/ImageWidgets/GridView.h
	Widgets/ImageWidgets/GridWindow.cpp
	Widgets/ImageWidgets/GridWindow.h
	Widgets/ImageWidgets/HistogramPicker.cpp
	Widgets/ImageWidgets/HistogramPicker.h
	Widgets/ImageWidgets/HistogramView.cpp
	Widgets/ImageWidgets/HistogramView.h
	Widgets/ImageWidgets/HistogramWindow.cpp
	Widgets/ImageWidgets/HistogramWindow.h
	Widgets/ImageWidgets/ImageContextMenu.cpp
	Widgets/ImageWidgets/ImageContextMenu.h
	Widgets/ImageWidgets/ImageView.cpp
	Widgets/ImageWidgets/ImageView.h
	Widgets/ImageWidgets/ImageViewer.cpp
	Widgets/ImageWidgets/ImageViewer.h
	Widgets/ImageWidgets/ImageWidget.cpp
	Widgets/ImageWidgets/ImageWidget.h
	Widgets/ImageWidgets/ImageWindow.cpp
	Widgets/ImageWidgets/ImageWindow.h
	Widgets/ImageWidgets/PixelGrid.cpp
	Widgets/ImageWidgets/PixelGrid.h
	Widgets/ImageWidgets/ProjectionHistogramView.cpp
	Widgets/ImageWidgets/ProjectionHistogramView.h
	Widgets/ImageWidgets/ProjectionHistogramWindow.cpp
	Widgets/ImageWidgets/ProjectionHistogramWindow.h
	Widgets/ImageWidgets/RowView.cpp
	Widgets/ImageWidgets/RowView.h
	Widgets/ImageWidgets/RowWindow.cpp
	Widgets/ImageWidgets/RowWindow.h
	Widgets/ImageWidgets/StandardImageWindow.cpp
	Widgets/ImageWidgets/StandardImageWindow.h
	Widgets/ImageWidgets/ThumbnailView.cpp
	Widgets/ImageWidgets/ThumbnailView.h
	Widgets/ImageWidgets/ZoomViewer.cpp
	Widgets/ImageWidgets/ZoomViewer.h
	Widgets/NavBar/ImageDelegate.cpp
	Widgets/NavBar/ImageDelegate.h
	Widgets/NavBar/NavigationBar.cpp
	Widgets/NavBar/NavigationBar.h
	Widgets/NavBar/NavigationDock.cpp
	Widgets/NavBar/NavigationDock.h
	Widgets/NavBar/NodeListModel.cpp
	Widgets/NavBar/NodeListModel.h
)

if(NOT CMAKE_BUILD_TYPE MATCHES Debug)
	# When the project is built in Release mode (or RelWithDebInfo, MinSizeRel, or nothing), we 'touch' AboutDialog.cpp.
	# With this trick, the file will always be built, and the constants __DATE__ and __TIME__ it contains will be replaced
	# by the real compilation date and time.
	add_custom_target(DoRebuildAbout ALL ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_SOURCE_DIR}/Widgets/AboutDialog.cpp)
endif()

qt5_add_resources(RCC_FILES "res.qrc")

set(TS_FILES genericinterface_en.ts genericinterface_fr.ts)
# Uncomment only 1 of the 2 folowing commands, depending on your needs
# -1- Use current translations files
qt5_add_translation(QM_FILES ${TS_FILES})
# -2- Update translations files with changes from c++ sources
#qt5_create_translation(QM_FILES ${GenInterface_SOURCES} ${TS_FILES})
# -3- Update translations files with changes from c++ sources AND drop useless translations
#qt5_create_translation(QM_FILES ${GenInterface_SOURCES} ${TS_FILES} OPTIONS -no-obsolete)

if(NOT WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

add_library(GenericInterface OBJECT ${GenInterface_SOURCES} ${RCC_FILES})

# This target is defined for convenience only, and shouldn't be used to link with another target.
add_library(static_GenericInterface STATIC $<TARGET_OBJECTS:GenericInterface> ${TS_FILES} ${QM_FILES})

add_custom_command(TARGET static_GenericInterface POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory ${DEPLOY_OUTPUT_DIRECTORY}/lang
	COMMENT "Create directory ${DEPLOY_OUTPUT_DIRECTORY}/lang")
foreach(__FILE ${QM_FILES})
	add_custom_command(TARGET static_GenericInterface POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${__FILE} ${DEPLOY_OUTPUT_DIRECTORY}/lang
		COMMENT "Copy ${__FILE} to ${DEPLOY_OUTPUT_DIRECTORY}/lang")
endforeach()

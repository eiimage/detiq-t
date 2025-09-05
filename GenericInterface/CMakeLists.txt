cmake_minimum_required(VERSION 3.21)
project(GenericInterface LANGUAGES C CXX)
include_directories(../ImageIn)

# Auto-génération (moc/uic/rcc) côté sous-projet
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qwt 6.0 REQUIRED)
if(QWT_FOUND)
	include_directories(${QWT_INCLUDE_DIRS})
else()
	message(FATAL_ERROR "Cannot find Qwt library. Please add the Qwt library directory to CMAKE_PREFIX_PATH.")
endif()

# Find the Qt libraries
find_package(Qt6Core REQUIRED)
find_package(Qt6Gui REQUIRED)
find_package(Qt6Widgets REQUIRED)
find_package(Qt6Svg REQUIRED)
find_package(Qt6OpenGL REQUIRED)
find_package(Qt6OpenGLWidgets REQUIRED)
find_package(Qt6PrintSupport REQUIRED)
find_package(Qt6LinguistTools REQUIRED)


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
        Widgets/KeyboardShortcut.cpp
        Widgets/KeyboardShortcut.h
	Widgets/ImageWidgets/AlternativeImageView.cpp
	Widgets/ImageWidgets/AlternativeImageView.h
	Widgets/ImageWidgets/ColumnView.cpp
	Widgets/ImageWidgets/ColumnView.h
	Widgets/ImageWidgets/ColumnWindow.cpp
	Widgets/ImageWidgets/ColumnWindow.h
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
        Widgets/ImageWidgets/MyDoubleValidator.h
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

qt_add_resources(RCC_FILES "res.qrc")
set(TS_FILES genericinterface_en.ts genericinterface_fr.ts)
# Uncomment only 1 of the 2 folowing commands, depending on your needs
# -1- Use current translations files
#qt_add_translation(QM_FILES ${TS_FILES})
# -2- Update translations files with changes from c++ sources
qt_create_translation(QM_FILES ${GenInterface_SOURCES} ${TS_FILES})
# -3- Update translations files with changes from c++ sources AND drop useless translations
#qt_create_translation(QM_FILES ${GenInterface_SOURCES} ${TS_FILES} OPTIONS -no-obsolete)

if(NOT WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

add_library(GenericInterface OBJECT ${GenInterface_SOURCES} ${RCC_FILES})

# This target is defined for convenience only, and shouldn't be used to link with another target.
add_library(static_GenericInterface STATIC $<TARGET_OBJECTS:GenericInterface> ${TS_FILES} ${QM_FILES})

# Inclure/lier Qt6 + Qwt pour cette cible
target_include_directories(GenericInterface PRIVATE
		  ${CMAKE_CURRENT_SOURCE_DIR}
		  ${CMAKE_CURRENT_BINARY_DIR}
		  ${QWT_INCLUDE_DIRS}
		  ${QWT_INCLUDE_DIR}        # au cas où FindQwt ne renseigne que celui-ci
		  ${QWT_INCLUDE_DIR}/qwt    # layout avec sous-dossier
		  ${QWT_INCLUDE_DIR}/qwt6   # certaines builds emballent les headers ici
		)

target_link_libraries(GenericInterface PRIVATE
  Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Svg Qt6::PrintSupport
  Qt6::OpenGL Qt6::OpenGLWidgets
  ${QWT_LIBRARIES}
)
add_custom_command(TARGET static_GenericInterface POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory ${DEPLOY_OUTPUT_DIRECTORY}/lang
	COMMENT "Create directory ${DEPLOY_OUTPUT_DIRECTORY}/lang")
foreach(__FILE ${QM_FILES})
	add_custom_command(TARGET static_GenericInterface POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${__FILE} ${DEPLOY_OUTPUT_DIRECTORY}/lang
		COMMENT "Copy ${__FILE} to ${DEPLOY_OUTPUT_DIRECTORY}/lang")
endforeach()

cmake_minimum_required(VERSION 3.16)
project(Detiq-t)


# Add CMake modules path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

# Pont Qwt depuis le parent
if(DEFINED QWT_INCLUDE_DIR AND DEFINED QWT_LIBRARY)
    set(QWT_INCLUDE_DIR "${QWT_INCLUDE_DIR}" CACHE PATH "Path to Qwt headers" FORCE)
    set(QWT_LIBRARY     "${QWT_LIBRARY}"     CACHE FILEPATH "Path to Qwt library" FORCE)
    get_filename_component(_QWT_LIBDIR "${QWT_LIBRARY}" DIRECTORY)
    get_filename_component(_QWT_ROOT   "${_QWT_LIBDIR}" DIRECTORY)
    set(QWT_ROOT_DIR "${_QWT_ROOT}" CACHE PATH "Qwt root prefix" FORCE)
    message(STATUS "Detiq-t: QWT_INCLUDE_DIR=${QWT_INCLUDE_DIR}")
    message(STATUS "Detiq-t: QWT_LIBRARY=${QWT_LIBRARY}")
endif()

add_subdirectory(ImageIn)
add_subdirectory(GenericInterface)
add_subdirectory(Test)

# This should be uncommented when compilation issues will be fixed
# add_subdirectory(Applications)

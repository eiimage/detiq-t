project(Detiq-t)
cmake_minimum_required(VERSION 2.8.11)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Add CMake modules path
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

add_subdirectory(ImageIn)
add_subdirectory(GenericInterface)

# This should be uncommented when compilation issues will be fixed
# add_subdirectory(Applications)

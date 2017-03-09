cmake_minimum_required(VERSION 2.8.3)
project(msf_updates)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
set(CMAKE_BUILD_TYPE Release)

#build with boost 1.6x
add_definitions(-DBOOST_NO_CXX11_VARIADIC_TEMPLATES)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/)

find_package(Eigen3 REQUIRED)
include_directories(include ../msf_core/include ../msf_timing/include ../msf_core/src ${EIGEN3_INCLUDE_DIR})
MESSAGE(STATUS "Eigen include: " ${EIGEN3_INCLUDE_DIR})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w") #-Wall -Wextra -Wno-unused-parameter")
if (CMAKE_COMPILER_IS_GNUCC)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../lib)
add_library(${PROJECT_NAME} SHARED 
    src/msf_distort/PoseDistorter.cc
    src/pose_msf/pose_sensormanager.cc
    )
target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/../lib/libmsf_core.so)

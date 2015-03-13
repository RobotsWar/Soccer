cmake_minimum_required(VERSION 2.8)
project(Soccer)

set (RHOSH_DIR "${CMAKE_SOURCE_DIR}/../../Rhosh"
    CACHE STRING "Rhosh directory")

set (HARDWARE "${CMAKE_CURRENT_SOURCE_DIR}/../Hardware"
    CACHE "Hardware directory" STRING)

set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wno-reorder -pedantic")

add_subdirectory(${RHOSH_DIR} rhosh)
include_directories(${RHOSH_DIR})

include_directories(${HARDWARE})

set (SOURCES
    main.cpp
    locomotion.cpp
    kinematic.cpp
    function.cpp
    ${HARDWARE}/HardwareClient.cpp
    ${HARDWARE}/HardwareImage.cpp
)

#Build Vision test
add_executable(AI ${SOURCES})
target_link_libraries(AI zmq rhosh)

cmake_minimum_required(VERSION 2.8)
project(Soccer)

set (HARDWARE "${CMAKE_CURRENT_SOURCE_DIR}/../Hardware"
    CACHE "Hardware directory" STRING)

set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wno-reorder -pedantic -U__STRICT_ANSI__")

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
target_link_libraries(AI zmq)

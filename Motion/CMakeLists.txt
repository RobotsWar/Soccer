cmake_minimum_required(VERSION 2.8)
project(Soccer)

set(SOURCES_FILES_CPP
    Robot.cpp
    serialib.cpp
)

#Use OpenCV and v4l2
set(EXTRA_LIBS
    ${OpenCV_LIBS}
    v4l2
)

#Build Vision test
add_executable(robot ${SOURCES_FILES_CPP} robot.cpp)

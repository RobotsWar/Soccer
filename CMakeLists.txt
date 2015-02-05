cmake_minimum_required(VERSION 2.8)
project(Soccer)

include_directories(".")

set(SOURCES_FILES_CPP
    main.cpp
    
    Vision/calibrate.cpp
    Vision/vision-tim.cpp
    Vision/Sensor.cpp

    Control/Controller.cpp
    Control/MainLoop.cpp

    Motion/Robot.cpp
    Motion/serialib.cpp

    Utils/params.cpp
)

ADD_DEFINITIONS (
    -std=c++11
)

#Look for OpenCV Lib
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_DIR}/include)
include_directories(${OpenCV_DIR}/include/opencv2)
link_directories(${OpenCV_DIR}/lib ${OpenCV_DIR}/3rdparty/lib)

#Use OpenCV and v4l2
set(EXTRA_LIBS
    ${OpenCV_LIBS}
    #opencv_highgui
    #v4l2.0
)

#Build Vision test
add_executable(soccer ${SOURCES_FILES_CPP})
target_link_libraries(soccer ${EXTRA_LIBS})



cmake_minimum_required(VERSION 2.8)
project(Soccer)

set(SOURCES_FILES_CPP
    main.cpp
)

#Look for OpenCV Lib
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_DIR}/include)
include_directories(${OpenCV_DIR}/include/opencv2)
link_directories(${OpenCV_DIR}/lib ${OpenCV_DIR}/3rdparty/lib)

#Use OpenCV and v4l2
set(EXTRA_LIBS
    ${OpenCV_LIBS}
    v4l2
)

#Build Vision test
add_executable(testVision "main.cpp")
target_link_libraries(testVision ${EXTRA_LIBS})


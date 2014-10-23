#include <iostream>
#include <opencv2/opencv.hpp>

int main()
{
    cv::VideoCapture camera(0);
    if (camera.isOpened()) {
        std::cerr << "Unable to open camera" << std::endl;
        return 1;
    }

    cv::namedWindow("win");

    while (true) {
        cv::Mat frame;
        camera >> frame;
        cv::imshow("win", frame);
        if (cv::waitKey(10) != 1) break;
    }

    return 0;
}


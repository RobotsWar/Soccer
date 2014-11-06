#include <iostream>
#include <opencv2/opencv.hpp>

void tim();

cv::Mat frame;

void onMouse(int event, int x, int y, int, void*)
{
    if (event == cv::EVENT_LBUTTONDOWN) {
        cv::Vec3b& pixel = frame.at<cv::Vec3b>(y,x);
        std::cout << "Pixel at [" << x << "," << y << "] = ";
        std::cout << (int)pixel[0] << ",";
        std::cout << (int)pixel[1] << ",";
        std::cout << (int)pixel[2] << std::endl;
    }
}

int main()
{
   //tim();

    cv::VideoCapture camera(1);
    if (!camera.isOpened()) {
        std::cerr << "Unable to open camera" << std::endl;
        return 1;
    }

    cv::namedWindow("win");

    cv::setMouseCallback("win", onMouse, NULL);

    while (true) {
        camera >> frame;

        long sumX = 0;
        long sumY = 0;
        long count = 0;
        for (int i=0;i<frame.rows;i++) {
            for (int j=0;j<frame.cols;j++) {
                cv::Vec3b& pixel = frame.at<cv::Vec3b>(i,j);
                if (
                    pixel[2] > 100 && 
                    pixel[0] < 100 && 
                    pixel[1] < 100
                ) {
                    pixel[0] = 255;
                    pixel[1] = 255;
                    pixel[2] = 255;
                    sumX += j;
                    sumY += i;
                    count++;
                } else {
                    pixel[0] = 0;
                    pixel[1] = 0;
                    pixel[2] = 0;
                }
            }
        }
        if (count > 0) {
            int x = sumX/count;
            int y = sumY/count;
            cv::circle(frame, cv::Point(x, y), 10, cv::Scalar(0, 0, 255), -1);
        }

        cv::imshow("win", frame);
        if (cv::waitKey(10) != -1) break;
    }

    return 0;
}


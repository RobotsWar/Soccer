#pragma once 

#include <functional>
#include <opencv2/opencv.hpp>

void calibrate(params & p
	       ,std::function<void(params &, const cv::Mat & src, cv::Mat & dst)> feedback);



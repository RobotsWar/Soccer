#pragma once 

#include "Utils/params.hpp"

#include <opencv2/opencv.hpp>

class FrameListener {
public:
   virtual void onFrame(const params&, const cv::Mat& src, double time, cv::Mat * feedback = nullptr) = 0;
};

class MainLoop
{
public:
void register_(FrameListener & l);
void unregister(FrameListener& l);
   
int  run(const params&, int camera);
void stop();

private:
bool StopRequired;
cv::VideoCapture Camera;
std::set<FrameListener*> Listeners;
};

#include "Sensor.hpp"
#include "params.hpp"


PositionSensor::PositionSensor()
   : X(-1), Y(-1)
   , FromX(-1), FromY(-1)
   , Since(-1)
   , Interval(0.3)
   , Speed(0)
   , Vision(nullptr)
{}

void PositionSensor::update(const params& p, const cv::Mat& src, double time, cv::Mat* feedback)
{
   int x, y;
   Vision(p, src, x, y, feedback);
   stab(X, x, p[BallStab]);
   stab(Y, y, p[BallStab]);

   if (time-Since > Interval)
   {
      const double dx = FromX - X;
      const double dy = FromY - Y;

      Speed = std::sqrt(dx*dx + dy*dy) / (time-Since);

      FromX = X;
      FromY = Y;

      Since = time;
   }

#ifndef ONBOARD
   if (feedback != nullptr)
   {
      cv::circle(*feedback, cv::Point(X,Y), 20, cv::Scalar(0,0,255), 3);//, int thickness=1, int lineType=8, int shift=0)
      cv::putText(*feedback, std::to_string(Speed), cvPoint(X+15,Y-15), cv::FONT_HERSHEY_SIMPLEX,  0.35, cv::Scalar(0,0,255), 1.2, CV_AA);
   }
#endif
}


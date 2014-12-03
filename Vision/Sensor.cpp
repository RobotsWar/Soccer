#include "Sensor.hpp"





void PositionSensor::update(const params&, const cv::Mat& src, double time, cv::Mat* feedback = nullptr)
{
   int x, y;
   Vision(*params, src, x, y, feedback);
   stab(X, x, param[]);
   stab(Y, y, param[]);

   if (time-Since > Interval)
   {
      const double dx = FromX - X;
      const double dy = FromY - Y;
      Speed = std::sqrt(dx*dx + dy*dy) / (time-Since);
   }
}


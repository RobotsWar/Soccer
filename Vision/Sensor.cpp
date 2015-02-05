#include "Sensor.hpp"
#include "Utils/params.hpp"


void Sensor::register_(SensorListener & l) {
   Listeners.insert(&l);
}
void Sensor::unregister(SensorListener & l) {
   Listeners.erase(&l);
}

void Sensor::notify(double time, void* sender) const
{
   for (SensorListener* l : Listeners)
   {
      l -> onSensor(time, sender);
   }
}


PositionSensor::PositionSensor(position_vision vision, double interval)
   : X(-1), Y(-1)
   , FromX(-1), FromY(-1)
   , Since(-1)
   , Interval(interval)
   , Speed(-1)
   , Vision(vision)
{}

void PositionSensor::onFrame(const params& p, const cv::Mat& src, double time, cv::Mat* feedback)
{
   bool change = false;

   int x_, y_;
   Vision(p, src, x_, y_, feedback);

   if (x_ > 0 and y_ > 0) // l'object est detecté
   {
      const double x = x_ / (double) src.cols;
      const double y = y_ / (double) src.rows;

      if (X < 0) // l'objet n'était précédemment pas detecté
      {
         X = x;
         Y = y;
         change = true;
      }
      else // l'objet était précédemment detecté
      {
         // on stabilise la position
         if (abs(X - x) > p[BallStab]) {
            X = x;
            change = true;
         }
         if (abs(Y - y) > p[BallStab]) {
            Y = y;
            change = true;
         }
      }

      if (time-Since > Interval)
      {
         if (FromX >= 0)
         {
            const double dx = FromX - X;
            const double dy = FromY - Y;

            Speed = std::sqrt(dx*dx + dy*dy) / (time-Since);
            change = true;
         }

         FromX = X;
         FromY = Y;

         Since = time;
      }
   }
   else // l'objet n'est pas detecté
   {
      if (X >= 0) // l'object était précédemment détecté
         change = true;

      X = -1;
      Y = -1;

      FromX = -1;
      FromY = -1;
      Since = -1;
      Speed = -1;
   }

   if (change)
      notify(time, this);

#ifndef ONBOARD
   if (feedback != nullptr)
   {
      if (X >= 0 and Y >= 0)
      {
         cv::circle(*feedback, cv::Point(X,Y), 20, cv::Scalar(0,0,255), 3);//, int thickness=1, int lineType=8, int shift=0)
         cv::putText(*feedback, std::to_string(Speed), cvPoint(X+15,Y-15), cv::FONT_HERSHEY_SIMPLEX,  0.35, cv::Scalar(0,0,255), 1.2, CV_AA);
      }
   }
#endif
}


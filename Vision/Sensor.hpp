#pragma once

#include "params.hpp"

#include <opencv2/opencv.hpp>


template <typename T>
T abs(T x) { return (x < 0) ? -x : x; }

template <typename T>
void stab(T& old, const T& new_, const T& max)
{
   if (abs<>(old-new_) > max)
      old = new_;
}


// template <typename T>
// class Stabilized
// {
// public:
//    T Value;
//    T Delta;

//    Stabilized(T v, T d) : Value(v), Delta(d)
//    {}

//    void set(T value) {
//       if (abs<>(Value-value) >= Delta)
// 	 Value = value;
//    }
// };





typedef void (*position_vision)(const params&, const cv::Mat& src, int& x, int& y, cv::Mat * feedback);
// on pourra généraliser ça avec une interface Sensor 
class PositionSensor // : public Sensor
{
public:
   /* en pixels */
   int X;
   int Y;
   //bool Unknown = true;
   
   int FromX;
   int FromY;
   double Since;
   
   /* en secondes */
   double Interval;

   /* en pixels/seconde */
   double Speed;

   position_vision Vision;

   PositionSensor();
   
   /* virtual */ void update(const params&, const cv::Mat& src, double time, cv::Mat * feedback = nullptr);
};


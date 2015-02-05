#pragma once

#include "Utils/params.hpp"
#include "Control/MainLoop.hpp"

#include <opencv2/opencv.hpp>
#include <list>

template <typename T>
inline T abs(T x) { return (x < 0) ? -x : x; }


class SensorListener {
public:
   virtual void onSensor(double time, void* sender) = 0;
};

class Sensor : public FrameListener
{
public:
   void register_(SensorListener&);
   void unregister(SensorListener&);

protected:
   void notify(double time, void* sender) const;

private:
   std::set<SensorListener*> Listeners;
};




typedef void (*position_vision)(const params&, const cv::Mat& src, int& x, int& y, cv::Mat * feedback);
class PositionSensor : public Sensor
{
public: // faudrait encapsuler un peu...
   // toutes les positions sont entre 0 et 1
   // 0,0 en bas à gauche ; 1,1 en haut à droite
   double X;
   double Y;
   //bool Unknown = true;
   
   double FromX;
   double FromY;
   double Since;
   
   /* en secondes */
   double Interval;

   /* en unités/seconde */
   double Speed;

   position_vision Vision;
   
   PositionSensor(position_vision vision, double interval = 0.3);
   
   void onFrame(const params&, const cv::Mat& src, double time, cv::Mat * feedback = nullptr) override;
};


//// overkill à priori
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
//
// template <typename T>
// inline T stab(T old, T new_, T dmax, bool & updated)
// {
//    if (abs<>(old-new_) > dmax)
//    {
//       updated = true;
//       return new_;
//    }
//    else
//    {
//       updated = false;
//       return old;
//    }
// }

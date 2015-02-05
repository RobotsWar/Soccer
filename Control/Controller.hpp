#pragma once

#include "Utils/utils.hpp"

#include "Motion/Robot.h"

#include "Control/MainLoop.hpp"

#include "Vision/Sensor.hpp"

class DemoController : public SensorListener, public FrameListener
{
private:
   MainLoop & Loop;
   Robot Motion;
   PositionSensor Ball;

   enum {
      SEARCHING_BALL
      ,RUNNING_TO_BALL
   } State = SEARCHING_BALL;

   
public:
   DemoController(MainLoop &);

   void onSensor(double time, void* sender) override;
   void onFrame(const params&, const cv::Mat& src, double time, cv::Mat * feedback = nullptr) override;
};

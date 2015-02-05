#pragma once

#include <string>

// rustique mais pratique à sérialiser/envoyer ?
enum param
{
   Resize

   /* inRange terrain */
   ,BgAvgR
   ,BgAvgG
   ,BgAvgB

   ,BgStdDevR
   ,BgStdDevG
   ,BgStdDevB

   ,BgStdDevCoef


   /* inRange balle */
   ,BallAvgR
   ,BallAvgG
   ,BallAvgB

   ,BallStdDevR
   ,BallStdDevG
   ,BallStdDevB

   ,BallStdDevCoef


   /* Dilate/Erode Terrain */
   ,BgDilateIterations
   ,BgErodeIterations
   ,BgErodeSize
   ,BgDilateSize

   /* Stabilisation balle */
   ,BallStab


   ,END
};

typedef int params[param::END];
const int PARAM_MAX = 1000;

void defaultParams(params & p);
std::string to_string(const params & p);
bool readParams(params & p, const std::string & str);

void loadParams(params & p, const std::string & file);
void saveParams(params & p, const std::string & file);


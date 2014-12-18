#include "params.hpp"
#include "Sensor.hpp"

#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>


void calibrate(params & p
	       ,std::function<void(params &, const cv::Mat & src, cv::Mat & dst)> feedback);


void processFrameTim(const params & p, const cv::Mat & src, int& x, int& y, cv::Mat * feedback);


int main()
{
   params p;

   std::string paramsFile = getenv("HOME");
   paramsFile += "/.test-vision-params";

   std::ifstream in(paramsFile.c_str());
   if (! in.is_open())
      defaultParams(p);
   else
   {
      std::string s;
      std::getline(in, s);
      readParams(p, s);
   }
   in.close();


   
   PositionSensor sensor;
   sensor.Vision = processFrameTim;
   

   calibrate(p,
	     [sensor] (params & p, const cv::Mat & src, cv::Mat & feedback) mutable
	     {
		// insert your glue code ici :)
		sensor.update(p, src, cv::getTickCount()/cv::getTickFrequency(), &feedback);
	     } );
   
   

   std::ofstream out(paramsFile.c_str());
   if (out.is_open())
      out << to_string(p);

   return EXIT_SUCCESS;
}


#include "Utils/params.hpp"

#include "Vision/Sensor.hpp"
#include "Vision/calibrate.hpp"
#include "Vision/vision.hpp"

#include <opencv2/opencv.hpp>

int main()
{
   params p;

   std::string paramsFile = getenv("HOME");
   paramsFile += "/.test-vision-params";

   loadParams(p, paramsFile);

   
   PositionSensor sensor(processFrameTim);
   

   calibrate(p,
	     [sensor] (params & p, const cv::Mat & src, cv::Mat & feedback) mutable
	     {
		// insert your glue code ici :)
		sensor.onFrame(p, src, cv::getTickCount()/cv::getTickFrequency(), &feedback);
	     });
   
   

   saveParams(p, paramsFile);

   return EXIT_SUCCESS;
}


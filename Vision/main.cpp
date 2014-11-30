#include "params.hpp"

#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

void calibrate(params & p
	       ,void (*feedback)(params &, const cv::Mat & src, cv::Mat & dst));

void processFrameTim(const params & p, const cv::Mat & src, cv::Mat * feedback);


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



   calibrate(p,
	     [] (params & p, const cv::Mat & src, cv::Mat & feedback)
	     {
		// insert your glue code ici :)
		processFrameTim(p, src, &feedback);
	     } );

   

   std::ofstream out(paramsFile.c_str());
   if (out.is_open())
      out << to_string(p);

   return EXIT_SUCCESS;
}


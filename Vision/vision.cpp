#include "params.hpp"

#include <opencv2/opencv.hpp>



void processFrame(const params & p, const cv::Mat & src, cv::Mat * feedback = nullptr)
{
   cv::Mat m1,m2; // pour limiter un peu la mémoire...

   cv::resize(src, m1, cv::Size( (p[Resize]*src.cols)/PARAM_MAX  + 1
   				 ,(p[Resize]*src.rows)/PARAM_MAX + 1  ));


   /* Extraction Terrain */
   #define CH(pre,post,s) (p[pre##Avg##post] + ((p[pre##StdDev##post]*p[pre##StdDevCoef]*5) / PARAM_MAX) * s)
   inRange(m1
   	   ,cv::Scalar( CH(Bg,B,-1), CH(Bg,G,-1), CH(Bg,R,-1) )
   	   ,cv::Scalar( CH(Bg,B,+1), CH(Bg,G,+1), CH(Bg,R,+1) )
   	   ,m2);


   const cv::Mat erodeElem = getStructuringElement( cv::MORPH_RECT
   						    ,cv::Size( (p[BgErodeSize]*50)/PARAM_MAX + 1
   							      ,(p[BgErodeSize]*50)/PARAM_MAX + 1 ));


   const cv::Mat dilateElem = getStructuringElement( cv::MORPH_RECT
   						     ,cv::Size( (p[BgDilateSize]*50)/PARAM_MAX + 1
   							       ,(p[BgDilateSize]*50)/PARAM_MAX + 1 ));


   cv::erode (m2, m1, erodeElem, cv::Point(-1,-1), p[BgErodeIterations]);
   cv::dilate(m1, m2, dilateElem, cv::Point(-1,-1), p[BgDilateIterations]);


   #ifndef ONBOARD
   if (feedback != nullptr)
   {
      cv::resize(m2, m1, cv::Size( src.cols
				   ,src.rows  ));
   
      (*feedback) = m1;
   }
   #endif
}

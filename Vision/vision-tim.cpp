#include "params.hpp"

#include <opencv2/opencv.hpp>



#define CH(pre,post,s) (p[pre##Avg##post] + ((p[pre##StdDev##post]*p[pre##StdDevCoef]*5) / PARAM_MAX) * s)

#define IN_RANGE(param,src,dst)						\
   inRange(src								\
	   ,cv::Scalar( CH(param,B,-1), CH(param,G,-1), CH(param,R,-1) ) \
	   ,cv::Scalar( CH(param,B,+1), CH(param,G,+1), CH(param,R,+1) ) \
	   ,dst)



void ball(const params & p, const cv::Mat & src, const cv::Mat & mask, int& x, int& y, cv::Mat * feedback);
void ground(const params & p, const cv::Mat & src, cv::Mat & mask);





void processFrameTim(const params & p, const cv::Mat & src, int& x, int& y, cv::Mat * feedback = nullptr)
{
   cv::Mat resized, bgMask;

   // cv::resize(src, resized, cv::Size( (p[Resize]*src.cols)/PARAM_MAX  + 1
   // 				      ,(p[Resize]*src.rows)/PARAM_MAX + 1  ));
   const double dsize = p[Resize]/ (double) PARAM_MAX;
   cv::resize(src, resized, cv::Size(), dsize, dsize);

   ground(p, resized, bgMask);


#ifndef ONBOARD
   cv::Mat feedbackBall;
   ball(p, resized, bgMask, x, y, &feedbackBall);
#else
   ball(p, resized, bgMask, x, y);
#endif

   x = (int) (x/dsize);
   y = (int) (y/dsize);

#ifndef ONBOARD
   if (feedback != nullptr)
   {
      cv::Mat bgMaskR, feedbackBallR, masked, mix;

      cv::resize(bgMask, bgMaskR, cv::Size( src.cols
					    ,src.rows  ));

      cv::resize(feedbackBall, feedbackBallR, cv::Size( src.cols
							,src.rows  ));


      double k = src.rows / (double)resized.rows;
      double xr = k * x;
      double yr = k * y;

      src.copyTo(masked, bgMaskR);
      //cvtColor(m1,m1, CV_GRAY2BGR);
      addWeighted( masked, 1, feedbackBallR, 0.3, 0.0, mix);
      //circle(mix, cv::Point(xr,yr), 20, cv::Scalar(0,0,255), 3);//, int thickness=1, int lineType=8, int shift=0)
   
      (*feedback) = mix;
   }
#endif
}


/* Extraction Terrain */
void ground(const params & p, const cv::Mat & src, cv::Mat & mask)
{
   cv::Mat m1,m2;
   
   IN_RANGE(Bg, src, m2);

   const cv::Mat erodeElem = getStructuringElement( cv::MORPH_RECT
   						    ,cv::Size( (p[BgErodeSize]*50)/PARAM_MAX + 1
							       ,(p[BgErodeSize]*50)/PARAM_MAX + 1 ));


   const cv::Mat dilateElem = getStructuringElement( cv::MORPH_RECT
   						     ,cv::Size( (p[BgDilateSize]*50)/PARAM_MAX + 1
								,(p[BgDilateSize]*50)/PARAM_MAX + 1 ));


   cv::erode (m2, m1, erodeElem, cv::Point(-1,-1), p[BgErodeIterations]);
   cv::dilate(m1, mask, dilateElem, cv::Point(-1,-1), p[BgDilateIterations]);
}


void ball(const params & p, const cv::Mat & src, const cv::Mat & mask, int& x, int& y, cv::Mat * feedback = nullptr)
{
   cv::Mat bin;
   
   IN_RANGE(Ball, src, bin);

   cv::bitwise_and(bin, mask, bin);

   long long sx = 0;
   long long sy = 0;
   long long count = 0;

   for(int row = 0; row < bin.rows; row++) {
      uchar* p = bin.ptr(row);
      for(int col = 0; col < bin.cols; col++)
      {
	 uchar & pix = *(p+col);
	 if (pix)
	 {
	    sx += col;
	    sy += row;
	    count++;
	 }
      }
   }

   count = (count > 0) ? count : 1;
   if (count > 0)
   {
      x = sx/count;
      y = sy/count;
   }
   else
   {
      x = -1;
      y = -1;
   }

#ifndef ONBOARD
   if (feedback != nullptr)
   {
      cv::Mat m1;
      cvtColor(bin,bin, CV_GRAY2BGR);
      (*feedback) = bin;
   }
#endif

}

#include <iostream>
#include <opencv2/opencv.hpp>
#include "tim.hpp"



cv::Vec3d operator/(const cv::Vec3d & a, const cv::Vec3d & b) {
   return cv::Vec3d( a[0]/b[0]
		     ,a[1]/b[1] 
		     ,a[2]/b[2] );
}
cv::Vec3d operator*(const cv::Vec3d & a, const cv::Vec3d & b) {
   return cv::Vec3d( a[0]*b[0]
		     ,a[1]*b[1] 
		     ,a[2]*b[2] );
}
cv::Vec3d sqrt(const cv::Vec3d & a) {
   return cv::Vec3d( sqrt(a[0])
		     ,sqrt(a[1]) 
		     ,sqrt(a[2]) );
}

std::pair<cv::Vec3d,cv::Vec3d> stdDev(std::list<cv::Vec3d> & samples)
{
   cv::Vec3d sum = std::accumulate(samples.begin(), samples.end(), cv::Vec3d(0,0,0));
   cv::Vec3d avg = sum / cv::Vec3d(samples.size(),samples.size(),samples.size());

   cv::Vec3d squaresSum = std::accumulate( samples.begin(), samples.end(), cv::Vec3d(0,0,0)
					   ,[] (const cv::Vec3d & x, const cv::Vec3d & y) {return x + y*y;} );
   cv::Vec3d squaresAvg = squaresSum / cv::Vec3d(samples.size(),samples.size(),samples.size());

   cv::Vec3d stdDev = sqrt( squaresAvg - avg*avg );


   return std::pair<cv::Vec3d,cv::Vec3d>(avg, stdDev);
}




// Slider::Slider(const std::string & name, const std::string & window)
// {
//    Name = name;
//    Window = window;
//    cv::createTrackbar(Name, Window, &Value, MAX, onSlide);
// }
// void Slider::onSlide(int raw, void*) {
//    Value = raw / (double)MAX;
// }
// const Slider & Slider::operator=(double value) {
//    Value = (int) (value * MAX);
//    setTrackbarPos(Name, Window, Value);
// }


// Calibrator::Calibrator()
//    :SliderResize("résolution", "controls")
//    ,SliderStdDev("coef. écart type", "controls")
//    ,SliderDilateIterations("itérations dilate", "controls")
//    ,SliderErodeIterations("itérations erode", "controls")
//    ,SliderErodeSize("taille kernel erode", "controls")
//    ,SliderDilateSize("taille kernel erode", "controls")
// {}

// void Calibrator::run(Parameters & p)
// {
//    // MAJ des controls
//    SliderResize = p.Resize;
//    SliderStdDev = p.StdDevCoef;
//    SliderDilateIterations = p.DilateIterations;
//    SliderErodeIterations  = p.ErodeIterations;
//    SliderErodeSize  = p.ErodeSize;
//    SliderDilateSize = p.DilateSize;


//    cv::imshow("controls");
// }


// void Calibrator::on( int, void* )
// {}
// void Calibrator::addControls()
// {
//    cv::createTrackbar("test", "", nullptr, SLIDER_MAX);
// }


// void calibrate(params &)
// {
// }


// BGR !

int greenFilterSlider;
std::list<cv::Vec3d> samples;



cv::Mat orig;
const int SLIDER_MAX = 200;


void onGreenFilterSlider( int, void* )
{}

void onMouse_(int event, int x, int y, int, void*)
{
   if (event == cv::EVENT_LBUTTONDOWN)
   {
      //const cv::Vec3b at = orig.at<cv::Vec3b>(y,x);
      samples.emplace_back( orig.at<cv::Vec3b>(y,x) );

      std::cout << "(" << x << "," << y << ")\t(" << (double) samples.back()[0] << ",";
      std::cout << samples.back()[1] << ",";
      std::cout << samples.back()[2] << ")" << std::endl;
   }
}



void tim()
{
   std::cout << "Hola !\n" "[e] pour étalonner\n" "[q] pour quitter\n\n";

   cv::VideoCapture camera(0);
   if (!camera.isOpened()) {
      std::cerr << "Unable to open camera" << std::endl;
      return;
   }


   cv::createTrackbar("agressivité filtre vert", "orig", &greenFilterSlider, SLIDER_MAX, onGreenFilterSlider);
   cv::setMouseCallback("orig", onMouse_, NULL);

   cv::namedWindow("orig");
   cv::namedWindow("result");


   std::pair<cv::Vec3d, cv::Vec3d> range_ = { {0,0,0}, {0,0,0} };

   bool stop = false;
   while (not stop)
   {
      cv::Mat resized, inranged, dilated, result;
      camera >> orig;
      const double ds = 0.3;
      cv::resize(orig, resized, cv::Size(0,0), ds, ds);

      double d = greenFilterSlider / (double) SLIDER_MAX;
      std::pair<cv::Vec3d, cv::Vec3d> range = { range_.first - d*4*range_.second, range_.first + d*4*range_.second };

      inRange(resized
	      , cv::Scalar((int)range.first[0] , (int)range.first[1] , (int)range.first[2])
	      , cv::Scalar((int)range.second[0], (int)range.second[1], (int)range.second[2])
	      , inranged);


      const cv::Mat element = getStructuringElement( cv::MORPH_RECT,
						     cv::Size( 4, 4 ));
      cv::dilate(inranged, dilated, element);


      cv::resize(dilated, result, cv::Size(0,0), 1/ds, 1/ds);

      cv::imshow("orig", orig);
      cv::imshow("result", result);

      switch (cv::waitKey(10))
      {
      	 case 'e': std::cout << "\ncliquez pour récupérer des échantillons\n" "puis [ESPACE] pour calibrer  [ "; break;
      	 case ' ': if (samples.size() > 0) { range_ = stdDev(samples); samples.clear(); } break;

      	 case 'q': stop=true; break;
	 default:;
      }
   }
}


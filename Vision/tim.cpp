#include <iostream>
#include <opencv2/opencv.hpp>

// BGR !

int greenFilterSlider;
std::list<cv::Vec3b> samples;

const int SLIDER_MAX = 500;

cv::Mat orig;

void onGreenFilterSlider( int, void* )
{}

void onMouse_(int event, int x, int y, int, void*)
{
   if (event == cv::EVENT_LBUTTONDOWN)
   {
      samples.push_back( orig.at<cv::Vec3b>(y,x) );
      //samples.push_back( cv::Vec3b() );
      std::cout << std::endl << (int)samples.front()[0] << ",";
      std::cout << (int)samples.front()[1] << ",";
      std::cout << (int)samples.front()[2] << " ";
   }
}

#define MAX_(a,b) ( ((a) > (b)) ? (a) : (b) )
#define MIN_(a,b) ( ((a) < (b)) ? (a) : (b) )
void calibrate(std::list<cv::Vec3b> & samples
	       ,double & minG
	       
	       ,double & maxR
	       ,double & maxB )
{
   std::cout << "] --> calibré avec (minG,maxR,maxB) = ";

   long double G = 0;
   long double R = 0;
   long double B = 0;
   
   unsigned n = 0;
   
   for (auto p : samples)
   {
      G += p[0];
      R += p[1];
      B += p[2];

      n++;
   }

   minG = (int) ((G/n) * 0.5);
   
   maxR = (int) MIN_( ((R/n) * 1.5), 255 );
   maxB = (int) MIN_( ((B/n) * 1.5), 255 );

   samples.clear();
   
   std::cout << "(" << minG << "," << maxR << "," << maxB << ")\n";
}

void tim()
{
   std::cout << "Hola !\n" "[e] pour étalonner\n" "[q] pour quitter\n\n";

   cv::VideoCapture camera(1);
   if (!camera.isOpened()) {
      std::cerr << "Unable to open camera" << std::endl;
      return;
   }

   cv::namedWindow("orig");
   cv::namedWindow("result");

   cv::createTrackbar("agressivité filtre vert", "orig", &greenFilterSlider, SLIDER_MAX, onGreenFilterSlider);
   cv::setMouseCallback("orig", onMouse_, NULL);

   double minG = 30;
   double maxR = 180;
   double maxB = 180;

   bool stop = false;
   while (not stop) {
      cv::Mat green;
      camera >> orig;

      double d = greenFilterSlider / (double) SLIDER_MAX;

      inRange(orig
	      , cv::Scalar(0                     , (int) (minG + (255-minG)*d)    , 0)
	      , cv::Scalar((int) (maxB*(1-d))    , 255                            , (int) (maxR*(1-d)))
	      , green);

      cv::imshow("orig", orig);
      cv::imshow("result", green);

      switch (cv::waitKey(10))
      {
      	 case 'e': std::cout << "\ncliquez pour récupérer des échantillons\n" "puis [ESPACE] pour calibrer  [ "; break;
      	 case ' ': if (samples.size() > 0) calibrate(samples, minG, maxR, maxB); break;

      	 case 'q': stop=true; break;
	 default:;
      }
   }
}


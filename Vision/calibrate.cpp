#include "Utils/params.hpp"

#include <iostream>
#include <list>
#include <opencv2/opencv.hpp>

void addControls();
void onMouse(int event, int x, int y, int, void*);
void onKey(char key);
bool nextCamera();
void drawInfo(cv::Mat & dst);
std::pair<cv::Vec3d,cv::Vec3d> stdDev(const std::list<cv::Vec3d> & samples);

/* Dans l'espace global pour OpenCV/HighGUI */
int * Params;
std::list<cv::Vec3d> Samples;
cv::Mat Src;

int CameraId = -1;
cv::VideoCapture Camera;

const std::string DEFAULT_STATUS = "[c] camera  [p] echantillonage  [q] quitter";
const std::string PICK_STATUS = "ECHANTILLONS  [g] inRange terrain  [b] inRange balle  [ESC] annuler";
   
enum {
   DEFAULT
   ,PICK
   ,PICKING
} State;

int PickFromX;
int PickFromY;

int PickToX;
int PickToY;

void calibrate(params & p
	       ,std::function<void(params &, const cv::Mat & src, cv::Mat & dst)> feedback)
{
   nextCamera();

   Params = p;
   cv::namedWindow("source"  );//, CV_GUI_EXPANDED);
   cv::namedWindow("feedback");//, CV_GUI_EXPANDED);
   cv::setMouseCallback("source", onMouse);

   addControls();
   //cv::displayStatusBar("source", DEFAULT_STATUS);

   cv::Mat fb;
   bool quit = false;
   while(not quit)
   {
      Camera >> Src;

      feedback(p, Src, fb);
      drawInfo(Src);

      cv::imshow("source"  , Src);
      cv::imshow("feedback", fb);

      const char k = cv::waitKey(10);
      onKey(k);
      if (k == 'q') quit = true;
   }

   cv::destroyAllWindows();
}


void addControls()
{
   cv::namedWindow("ctrl");

   cv::createTrackbar("Resize", "ctrl", &( Params[Resize] ), PARAM_MAX);

   /* Terrain */
   cv::createTrackbar("BgStdDevCoef"  , "ctrl", &( Params[BgStdDevCoef]   ), PARAM_MAX);

   cv::createTrackbar("BgDilateIterations", "ctrl", &( Params[BgDilateIterations] ), 200);
   cv::createTrackbar("BgErodeIterations" , "ctrl", &( Params[BgErodeIterations]  ), 200);
   cv::createTrackbar("BgDilateSize"      , "ctrl", &( Params[BgDilateSize]       ), PARAM_MAX);
   cv::createTrackbar("BgErodeSize"       , "ctrl", &( Params[BgErodeSize]        ), PARAM_MAX);

   /* Balle */
   cv::createTrackbar("BallStdDevCoef", "ctrl", &( Params[BallStdDevCoef] ), PARAM_MAX);
   cv::createTrackbar("BallStab", "ctrl", &( Params[BallStab] ), 30);
}



void onMouse(int event, int x, int y, int, void*)
{

   if (State == DEFAULT)
   {}
   else if (State == PICK && event == cv::EVENT_LBUTTONDOWN)
   {
      PickFromX = x;
      PickFromY = y;

      PickToX   = x;
      PickToY   = y;

      State = PICKING;
   }
   else if (State == PICKING)
   {
      PickToX = x;
      PickToY = y;

      if (event == cv::EVENT_LBUTTONUP)
      {
	 int fromX = std::min(PickFromX, PickToX);
	 int fromY = std::min(PickFromY, PickToY);
	 int toX = std::max(PickFromX, PickToX) + 1;
	 int toY = std::max(PickFromY, PickToY) + 1;

	 for(int x = fromX; x <= toX; x++)
	    for(int y = fromY; y <= toY; y++)
	    {
	       auto p = Src.at<cv::Vec3b>(y,x);
	       Samples.emplace_back(p[0], p[1], p[2]);
	    }

	 State = PICK;
      }
   }
}

void onKey(char key)
{
   /* Mode échantillonnage */
   if (key == 'p' and State == DEFAULT)
   {
      Samples.clear();
      State = PICK;
      //cv::displayStatusBar("source", PICK_STATUS);
   }

   /* calibrer le inRange du terrain avec les échantillons*/
   else if (key == 'g' and State == PICK)
   {
      State = DEFAULT;
      //cv::displayStatusBar("source", DEFAULT_STATUS);

      auto r = stdDev(Samples);
      Params[BgAvgB] = (int) r.first[0];
      Params[BgAvgG] = (int) r.first[1];
      Params[BgAvgR] = (int) r.first[2];

      Params[BgStdDevB] = (int) r.second[0];
      Params[BgStdDevG] = (int) r.second[1];
      Params[BgStdDevR] = (int) r.second[2];
   }

   /* calibrer le inRange de la balle avec les échantillons*/
   else if (key == 'b' and State == PICK)
   {
      State = DEFAULT;
      //cv::displayStatusBar("source", DEFAULT_STATUS);

      auto r = stdDev(Samples);
      Params[BallAvgB] = (int) r.first[0];
      Params[BallAvgG] = (int) r.first[1];
      Params[BallAvgR] = (int) r.first[2];

      Params[BallStdDevB] = (int) r.second[0];
      Params[BallStdDevG] = (int) r.second[1];
      Params[BallStdDevR] = (int) r.second[2];
   }

   else if (key == 27 and State == PICK)
   {
      State = DEFAULT;
   }

   else if (key == 'c' and State == DEFAULT)
      nextCamera();
}

bool nextCamera()
{
   int i = CameraId+1;
   do {
      Camera.open(i);
      if (Camera.isOpened())
      {
	 CameraId = i;
	 return true;
      }
     
      i = (i+1) % 10;
      
   } while (i != CameraId + 1);

   return false;
}

void drawInfo(cv::Mat & dst)
{
   if (State == PICKING)
   {
      cv::rectangle(dst
		    ,cv::Point(PickFromX,PickFromY)
		    ,cv::Point(PickToX,PickToY)
		    ,cv::Scalar(100,0,0), 2);
   }
   


   if (State == PICK or State == PICKING)
   {
      cv::putText(dst, PICK_STATUS, cvPoint(10,15), cv::FONT_HERSHEY_SIMPLEX,  0.35, cv::Scalar(100,0,0), 1.2, CV_AA);

      cv::putText(dst, std::to_string(Samples.size())
		  ,cvPoint(10,27), cv::FONT_HERSHEY_SIMPLEX,  0.4, cv::Scalar(150,0,0), 1.2, CV_AA);
   }
   else
      cv::putText(dst, DEFAULT_STATUS, cvPoint(10,15), cv::FONT_HERSHEY_SIMPLEX, 0.35, cv::Scalar(0,0,0), 1.2, CV_AA);
}





/* Utils */
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
   return cv::Vec3d( std::sqrt(a[0])
		     ,std::sqrt(a[1]) 
		     ,std::sqrt(a[2]) );
}

std::pair<cv::Vec3d,cv::Vec3d> stdDev(const std::list<cv::Vec3d> & samples)
{
   cv::Vec3d sum = std::accumulate(samples.begin(), samples.end(), cv::Vec3d(0,0,0));
   cv::Vec3d avg = sum / cv::Vec3d(samples.size(),samples.size(),samples.size());

   cv::Vec3d squaresSum = std::accumulate( samples.begin(), samples.end(), cv::Vec3d(0,0,0)
					   ,[] (const cv::Vec3d & a, const cv::Vec3d & b) {return a + b*b;} );
   cv::Vec3d squaresAvg = squaresSum / cv::Vec3d(samples.size(),samples.size(),samples.size());

   cv::Vec3d stdDev = sqrt( squaresAvg - avg*avg );


   return std::pair<cv::Vec3d,cv::Vec3d>(avg, stdDev);
}


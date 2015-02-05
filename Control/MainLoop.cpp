#include "Control/MainLoop.hpp"

void MainLoop::register_(FrameListener & l) {
   Listeners.insert(&l);
}
void MainLoop::unregister(FrameListener & l) {
   Listeners.erase(&l);
}

int MainLoop::run(const params& params, int cam)
{
   Camera.open(cam);

   // si échec de l'ouverture, on scan
   int i = cam + 1;
   while (i != cam and not Camera.isOpened())
   {
      Camera.open(i);
      i = (i+1) % 10;
   } 
   if (not Camera.isOpened())
      return EXIT_FAILURE;

   cv::Mat src;

   StopRequired = false;
   while(not StopRequired)
   {
      Camera >> src;

      for (FrameListener* l : Listeners)
      {
         l -> onFrame(params, src, cv::getTickCount()/cv::getTickFrequency(), nullptr);
      }
   }

   return EXIT_SUCCESS;
}

void MainLoop::stop() {
   StopRequired = true;
}






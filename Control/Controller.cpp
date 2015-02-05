#include "Control/Controller.hpp"

#include "Vision/vision.hpp"

DemoController::DemoController(MainLoop & ml)
   : Loop(ml)
   , Ball(processFrameTim)
{
   Loop.register_(Ball);
   Loop.register_(*this);

   Ball.register_(*this);
}

// le Observer/Observable est un peu bancale ici
// le top serai un mécanisme signals/slots, faut voir...
// utiliser un void* est un peu sale
// mais c'est pratique pour la comparaison :)
void DemoController::onSensor(double time, void* sender)
{
   if ( sender == (void*) &Ball )
   {
      if (State == SEARCHING_BALL)
      {
         // on arrête tout des qu'on voit la balle
         Motion.stopMoving();
         State = RUNNING_TO_BALL;
      }

      // on veut se déplacer de sorte d'amener la balle au milieu (x = 0.5)
      const float dx = Ball.X - 0.5;
      const float dy = 0.5 - dx;

      Motion.setVector(dx*2 * 10, dy*2 * 10);
   }
}

void DemoController::onFrame(const params&, const cv::Mat& src, double time, cv::Mat * feedback)
{}

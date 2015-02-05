#include "Utils/utils.hpp"
#include "Utils/params.hpp"

#include "Vision/calibrate.hpp"

#include "Control/Controller.hpp"

int main()
{
   // On peut faire un seul éxécutable et tout mettre ici avec des options à l'éxécution
   // calibration, Motion, DemoController...
   // et faire de la compilation conditionel (#ifndef ONBOARD)
   

   params p; // on peut utiliser ça pour autre chose que la vision 
   std::string paramsFile = getenv("HOME");
   paramsFile += "/.test-vision-params";

   loadParams(p, paramsFile);
   
   MainLoop Loop;
   DemoController Controller(Loop);

   return Loop.run(p, 0);
}


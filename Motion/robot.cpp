#include <iostream>
#include <getopt.h>
#include "Robot.h"

int main(int argc, char *argv[])
{
    int index;
    std::cout << "Connecting" << std::endl;
    Robot robot;

    while ((index = getopt(argc, argv, "gsx:y:t:h:r:")) != -1) {
        switch (index) {
            case 'g':
                std::cout << "Go!" << std::endl;
                robot.start();
                break;
            case 's':
                std::cout << "Stop!" << std::endl;
                robot.stop();
                break;
            case 'x': {
                float x = atof(optarg);
                std::cout << "X=" << x << std::endl;
                robot.setDx(x);
                      }
                break;
            case 'y': {
                float y = atof(optarg);
                std::cout << "Y=" << y << std::endl;
                robot.setDy(y);
                      }
                break;
            case 't': {
                float t = atof(optarg);
                std::cout << "T=" << t << std::endl;
                robot.setTurn(t);
                      }
                break;
            case 'r': {
                float r = atof(optarg);
                std::cout << "R=" << r << std::endl;
                robot.setR(r);
                      }
                break;
            case 'h': {
                float h = atof(optarg);
                std::cout << "H=" << h << std::endl;
                robot.setH(h);
                      }
                break;
        }
    }
}

#include <iostream>
#include <getopt.h>
#include "Robot.h"

void printUsage()
{
    std::cout << "Usage: " << std::endl;
    std::cout << "./robot" << std::endl;
    std::cout << "  -g: Go (runs the motors)" << std::endl;
    std::cout << "  -s: Stops (stops the motors)" << std::endl;
    std::cout << "  -x [dx]: Sets the robot X" << std::endl;
    std::cout << "  -y [dy]: Sets the robot Y" << std::endl;
    std::cout << "  -t [turn]: Sets the robot turn" << std::endl;
    std::cout << "  -r [radius]: Sets the robot radius" << std::endl;
    std::cout << "  -h [height]: Sets the robot height" << std::endl;
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    bool usage = true;
    int index;
    Robot robot;

    while ((index = getopt(argc, argv, "gsx:y:t:h:r:")) != -1) {
        switch (index) {
            case 'g':
                usage = false;
                std::cout << "Go!" << std::endl;
                robot.start();
                break;
            case 's':
                usage = false;
                std::cout << "Stop!" << std::endl;
                robot.stop();
                break;
            case 'x': {
                usage = false;
                float x = atof(optarg);
                std::cout << "X=" << x << std::endl;
                robot.setDx(x);
                      }
                break;
            case 'y': {
                usage = false;
                float y = atof(optarg);
                std::cout << "Y=" << y << std::endl;
                robot.setDy(y);
                      }
                break;
            case 't': {
                usage = false;
                float t = atof(optarg);
                std::cout << "T=" << t << std::endl;
                robot.setTurn(t);
                      }
                break;
            case 'r': {
                usage = false;
                float r = atof(optarg);
                std::cout << "R=" << r << std::endl;
                robot.setR(r);
                      }
                break;
            case 'h': {
                usage = false;
                float h = atof(optarg);
                std::cout << "H=" << h << std::endl;
                robot.setH(h);
                      }
                break;
        }
    }

    if (usage) {
        printUsage();
    }
}

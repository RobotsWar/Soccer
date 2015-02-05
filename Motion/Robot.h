#ifndef _ROBOT_H
#define _ROBOT_H

#include "serialib.h"

#define SERIAL_PORT     "/dev/ttyACM0"

class Robot
{
    public:
        Robot();
        virtual ~Robot();

        // Command on port
        void sendCommand(std::string command);
        void sendCommand(std::string command, float value);

        // Start/Stops the robot
        void start();
        void stop();

        // Moving control
        void setDx(float dx);
        void setDy(float dy);
        void setVector(float dx, float dy); // milimètres par pas
        void setTurn(float turn);           // degrés     par pas
        void stopMoving();

        // Posture
        void setH(float h);
        void setR(float r);

    protected:
        serialib port;
};

#endif

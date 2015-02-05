#include <string>
#include <sstream>
#include "Robot.h"
#include "serialib.h"

using namespace std;

Robot::Robot()
{
    char c = port.Open(SERIAL_PORT, 115200);
}

Robot::~Robot()
{
    port.Close();
}

void Robot::sendCommand(std::string command)
{
    std::stringstream ss;
    ss << command << "\n";
    port.WriteString(ss.str().c_str());
}

void Robot::sendCommand(std::string command, float value)
{
    std::stringstream ss;
    ss << command << " " << value;
    sendCommand(ss.str());
}

// Start/Stops the robot
void Robot::start()
{
    sendCommand("start");
}

void Robot::stop()
{
    sendCommand("stop");
}
        
void Robot::setDx(float dx)
{
    sendCommand("dx", dx);
}

void Robot::setDy(float dy)
{
    sendCommand("dy", dy);
}

void Robot::setH(float h)
{
    sendCommand("h", h);
}

void Robot::setR(float r)
{
    sendCommand("r", r);
}

// Moving control
void Robot::setVector(float dx, float dy)
{
    setDx(dx);
    setDy(dy);
}

void Robot::setTurn(float turn)
{
    sendCommand("turn", turn);
}

void Robot::stopMoving()
{
    sendCommand("dx", 0);
    sendCommand("dy", 0);
    sendCommand("turn", 0);
}

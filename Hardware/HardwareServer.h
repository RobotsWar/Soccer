#pragma once

#include <thread>
#include <map>
#include "HardwareImage.h"

class HardwareServer
{
    public:
        HardwareServer();

        float getAngle(int motor);
        void sendImage(HardwareImage &image);
        void runCommand();
        void run();

    protected:
        void *ctx;
        void *commandServer;
        void *imagesServer;
        std::map<int, float> motors;
        std::thread serverThread;
};

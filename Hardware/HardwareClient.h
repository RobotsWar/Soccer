#pragma once

#include <zmq.h>
#include <map>
#include <thread>
#include <string>
#include "HardwareImage.h"

class HardwareClient
{
    public:
        HardwareClient();
        void connect();

        void setAngle(int motor, float value);
        void imageProcess();
        HardwareImage getImage();
        void flush();

    protected:
        std::thread clientThread;
        void *ctx;
        void *imagesClient;
        void *commandClient;
        HardwareImage image;
        std::map<int, float> motors;
};

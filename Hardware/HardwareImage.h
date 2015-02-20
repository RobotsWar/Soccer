#pragma once

class HardwareImage
{
    public:
        static HardwareImage decode(std::string str);
        std::string encode();

        std::string data;
        unsigned int width;
        unsigned int height;
};

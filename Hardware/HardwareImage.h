#pragma once

class HardwareImage
{
    public:
        static HardwareImage decode(std::string str);
        std::string encode();

        int getR(int x, int y);
        int getG(int x, int y);
        int getB(int x, int y);
        void setColor(int x, int y, int col);
        void writePPM(std::string filename);

        std::string data;
        unsigned int width;
        unsigned int height;
};

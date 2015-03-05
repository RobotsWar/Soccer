#include <string>
#include "HardwareImage.h"

HardwareImage HardwareImage::decode(std::string str)
{
    HardwareImage img;
    unsigned int k = 0;
    img.width = 0;
    img.height = 0;

    img.width |= (str[k++]&0xff)<<24;
    img.width |= (str[k++]&0xff)<<16;
    img.width |= (str[k++]&0xff)<<8;
    img.width |= (str[k++]&0xff)<<0;

    img.height |= (str[k++]&0xff)<<24;
    img.height |= (str[k++]&0xff)<<16;
    img.height |= (str[k++]&0xff)<<8;
    img.height |= (str[k++]&0xff)<<0;

    for (; k<str.size(); k++) {
        img.data += str[k];
    }

    return img;
}

std::string HardwareImage::encode()
{
    std::string buffer;
    
    buffer += ((width>>24)&0xff);
    buffer += ((width>>16)&0xff);
    buffer += ((width>>8)&0xff);
    buffer += ((width>>0)&0xff);

    buffer += ((height>>24)&0xff);
    buffer += ((height>>16)&0xff);
    buffer += ((height>>8)&0xff);
    buffer += ((height>>0)&0xff);

    buffer += data;

    return buffer;
}

int HardwareImage::getV(int index)
{
    if (index < data.length()) {
        return data[index]&0xff;
    } else {
        return 0;
    }
}

int HardwareImage::getR(int x, int y)
{
    return getV((height-y-1)*width*3+3*x+0);
}

int HardwareImage::getG(int x, int y)
{
    return getV((height-y-1)*width*3+3*x+1);
}

int HardwareImage::getB(int x, int y)
{
    return getV((height-y-1)*width*3+3*x+2);
}
        
void HardwareImage::writePPM(std::string filename)
{
    FILE* f = fopen(filename.c_str(),"w");
    fprintf(f,"P3\n%d %d\n255\n", width, height);
    for (unsigned int y=0; y<height; y++) {
        for (unsigned int x=0; x<width; x++) {
            fprintf(f,"%hhu %hhu %hhu ", getR(x,y), getG(x,y), getB(x,y));
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

void HardwareImage::setColor(int x, int y, int col)
{
    if (((height-y-1)*width*3+3*x+2) < data.length()) {
        data[(height-y-1)*width*3+3*x+0] = (col>>16)&0xff;
        data[(height-y-1)*width*3+3*x+1] = (col>>8)&0xff;
        data[(height-y-1)*width*3+3*x+2] = (col>>0)&0xff;
    }
}

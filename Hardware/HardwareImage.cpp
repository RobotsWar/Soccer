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

    printf("Size: %d\n", str.size()-k);
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

int HardwareImage::getR(int x, int y)
{
    return data[(height-x-1)*128*3+3*y+0];
}

int HardwareImage::getG(int x, int y)
{
    return data[(height-x-1)*128*3+3*y+1];
}

int HardwareImage::getB(int x, int y)
{
    return data[(height-x-1)*128*3+3*y+2];
}
        
void HardwareImage::writePPM(std::string filename)
{
    FILE* f = fopen(filename.c_str(),"w");
    fprintf(f,"P3\n%d %d\n255\n", width, height);
    for (unsigned int x=0; x<width; x++) {
        for (unsigned int y=0; y<height; y++) {
            fprintf(f,"%hhu %hhu %hhu ", getR(x,y), getG(x,y), getB(x,y));
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

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

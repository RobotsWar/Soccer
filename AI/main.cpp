#include <iostream>
#include <HardwareClient.h>
#include <motors.h>
#include <unistd.h>
#include "locomotion.h"

using namespace std;

int main()
{
    float t = 0;
    HardwareClient client;
    client.connect();

    locomotion_init();
    locomotion_set_dx(0);
    locomotion_set_dy(0);

    while (true) {
        t += 0.02;
        locomotion_tick(t);

        // Getting image from the hardware
        HardwareImage img = client.getImage();

        // Do something clever here !
        // clever();

        if (img.width && img.height) {
            float xM = 0.0;
            int c = 0;
            for (unsigned int x=0; x<img.width; x++) {
                for (unsigned int y=0; y<img.height; y++) {
                    if (img.getR(x, y) > 200 && img.getG(x,y)<60 && img.getB(x,y)<60) {
                    img.setColor(x,y,0x0000ff);
                        xM += x;
                        c++;
                    }
                }
            }
            printf("%g\n", xM/c);
            img.writePPM("out.ppm");
        }

        // Sending angles to the hardware server
        for (unsigned int k=0; k<4; k++) {
            client.setAngle(3*k, l1[k]);
        }
        for (unsigned int k=0; k<4; k++) {
            client.setAngle(3*k+1, l2[k]);
        }
        for (unsigned int k=0; k<4; k++) {
            client.setAngle(3*k+2, l3[k]);
        }
        client.flush();

        usleep(20000);
    }
}

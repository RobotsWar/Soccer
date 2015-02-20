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
    locomotion_set_dx(60);
    locomotion_set_dy(-60);

    while (true) {
        t += 0.02;
        locomotion_tick(t);

        // Getting image from the hardware
        HardwareImage img = client.getImage();

        // Do something clever here !
        // clever();
        img.writePPM("out.ppm");

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

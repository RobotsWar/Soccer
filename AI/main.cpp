#include <math.h>
#include <iostream>
#include <HardwareClient.h>
#include <motors.h>
#include <unistd.h>
#include "locomotion.h"
#include <rhosh/terminal.h>
#include <rhosh/std.h>

using namespace std;
    
HardwareClient client;

int main()
{
    terminal_std_init();
    float dx=0, dy=0, turn=0;
    float sdx=0, sdy=0, sturn=0;
    float t = 0;
    client.connect();

    locomotion_init();
    locomotion_set_dx(0);
    locomotion_set_dy(0);
    locomotion_set_turn(0);

    while (true) {
        terminal_tick();

        t += 0.02;
        locomotion_tick(t);

        // Getting image from the hardware
        HardwareImage img = client.getImage();

        // Do something clever here !
        // clever();

        if (img.width && img.height) {
            img.writePPM("out.ppm");
            float xM = 0.0;
            int c = 0;
            for (unsigned int x=0; x<img.width; x++) {
                for (unsigned int y=0; y<img.height; y++) {
                    if (img.getR(x, y) > 200 && img.getG(x,y)<60 && img.getB(x,y)<60) {
                        xM += x;
                        c++;
                    }
                }
            }
            xM /= c;
            float xPos = (xM-(img.width/2))/(img.width/2);

            if (xPos==xPos && fabs(xPos) < 0.2) {
                turn = 0;
                dx = 60;
                dy = -60;
            } else {
                dx = 0;
                dy = 0;

                if (c > 10) {
                    turn = xPos*30;
                } else {
                    turn = 30;
                }
            }
        }
    
        // Smoothing locomotion
        sdx = sdx*0.8+dx*0.2;
        sdy = sdy*0.8+dy*0.2;
        sturn = sturn*0.8+turn*0.2;
        locomotion_set_dx(sdx);
        locomotion_set_dy(sdy);
        locomotion_set_turn(sturn+eturn);

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

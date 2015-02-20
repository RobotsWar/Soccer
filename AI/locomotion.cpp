#include <stdio.h>
#include <math.h>
#ifdef HAS_TERMINAL
#include <terminal.h>
#else
#define TERMINAL_PARAMETER_BOOL(name, desc, def) \
    bool name = def;
#define TERMINAL_PARAMETER_FLOAT(name, desc, def) \
    float name = def;
#define TERMINAL_PARAMETER_DOUBLE(name, desc, def) \
    double name = def;
#define TERMINAL_PARAMETER_INT(name, desc, def) \
    int name = def;
#endif
#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#endif
#include "config.h"
#include "function.h"
#include "locomotion.h"
#include "kinematic.h"

float l1[4];
float l2[4];
float l3[4];

float locomotion_get_motor(int idx)
{
    int c = (idx%3);
    switch (c) {
        case 0:
            return DEG2RAD(l1[idx/3]);
            break;
        case 1:
            return DEG2RAD(l2[idx/3]);
            break;
        case 2:
            return DEG2RAD(l3[idx/3]);
            break;
    }
}

// Amplitude multiplier
#define AMPLITUDE 30

// Legs bacakward mode
TERMINAL_PARAMETER_BOOL(backLegs, "Legs backwards", false);

// Body backward mode
TERMINAL_PARAMETER_BOOL(back, "Mode back", false);

// Amplitude & altitude of the robot
TERMINAL_PARAMETER_FLOAT(alt, "Height of the steps", 15.0);

// Static position
TERMINAL_PARAMETER_FLOAT(r, "Robot size", 90.0);
TERMINAL_PARAMETER_FLOAT(h, "Robot height", -55.0);

// Direction vector
TERMINAL_PARAMETER_FLOAT(dx, "Dx", 0.0);
TERMINAL_PARAMETER_FLOAT(dy, "Dy", 0.0);
TERMINAL_PARAMETER_FLOAT(crab, "Crab", 0.0);

// Turning, in ° per step
TERMINAL_PARAMETER_FLOAT(turn, "Turn", 0.0);

// Front delta h
TERMINAL_PARAMETER_FLOAT(frontH, "Front delta H", 0.0);

TERMINAL_PARAMETER_FLOAT(Ax, "Ax", 0.0);
TERMINAL_PARAMETER_FLOAT(Ay, "Ay", 0.0);
TERMINAL_PARAMETER_FLOAT(Az, "Az", 0.0);

#ifdef HAS_TERMINAL
TERMINAL_COMMAND(toggleBackLegs, "Toggle back legs")
{
    if (backLegs == 0) backLegs = 1;
    else if (backLegs == 1) backLegs = 0;
}

TERMINAL_COMMAND(toggleCrab, "Toggle crab mode")
{
    if (crab == 0) crab = 30;
    else if (crab == 30) crab = 0;
}
#endif

// Gait selector
#define GAIT_WALK       0
#define GAIT_TROT       1
TERMINAL_PARAMETER_INT(gait, "Gait (0:walk, 1:trot)", GAIT_TROT);

// Functions
Function rise;
Function step;

/**
 * Initializing functions
 */
void setup_functions()
{
    rise.clear();
    step.clear();
    
    if (gait == GAIT_WALK) {
        // Rising the legs
        rise.addPoint(0.0, 0.0);
        rise.addPoint(0.1, 1.0);
        rise.addPoint(0.3, 1.0);
        rise.addPoint(0.35, 0.0);
        rise.addPoint(1.0, 0.0);

        // Taking the leg forward
        step.addPoint(0.0, -0.5);
        step.addPoint(0.12, -0.5);
        step.addPoint(0.3, 0.5);
        step.addPoint(0.35, 0.5);
        step.addPoint(1.0, -0.5);
    }

    if (gait == GAIT_TROT) {
        // Rising the legs
        rise.addPoint(0.0, 0.0);
        rise.addPoint(0.1, 1.0);
        rise.addPoint(0.4, 1.0);
        rise.addPoint(0.5, 0.0);
        rise.addPoint(1.0, 0.0);

        // Taking the leg forward
        step.addPoint(0.0, -0.5);
        step.addPoint(0.1, -0.5);
        step.addPoint(0.4, 0.5);
        step.addPoint(1.0, -0.5);
    }
}

TERMINAL_PARAMETER_FLOAT(smoothBackLegs, "Smooth 180", 0.0);
TERMINAL_PARAMETER_FLOAT(smoothBack, "Smooth Back", -1.0);

void locomotion_init()
{
    back = (initialOrientation != 0);
    if (back) smoothBack = 1;
}
    
void locomotion_tick(float t)
{
    // Setting up functions
    setup_functions();

    // Smoothing 180
    if (backLegs && smoothBackLegs < 1) {
        smoothBackLegs += 0.02;
    }
    if (!backLegs && smoothBackLegs > 0) {
        smoothBackLegs -= 0.02;
    }
    if (back && smoothBack < 1) {
        smoothBack += 0.04;
    }
    if (!back && smoothBack > -1) {
        smoothBack -= 0.04;
    }
        
    for (int i=0; i<4; i++) {
        // Defining in which group of opposite legs this leg is
        bool group = ((i&1)==1);

        // This defines the phase of the gait
        float legPhase;
       
        if (gait == GAIT_WALK) {
            float phases[] = {0.0, 0.5, 0.75, 0.25};
            legPhase = t + phases[i];
        }
        if (gait == GAIT_TROT) {
            legPhase = t + group*0.5;
        }

        float x, y, z, a, b, c;

        // Computing the order in the referencial of the body
        float xOrder = step.getMod(legPhase)*dx;
        float yOrder = step.getMod(legPhase)*dy;

        // Computing the order in the referencial of the leg
        float bodyAngle = -(i*M_PI/2.0 - (M_PI/4.0))*smoothBack;
        if (group) {
            bodyAngle -= DEG2RAD(crab*(-smoothBack));
        } else {
            bodyAngle += DEG2RAD(crab*(-smoothBack));
        }
        float vx = xOrder*cos(bodyAngle)-yOrder*sin(bodyAngle);
        float vy = xOrder*sin(bodyAngle)+yOrder*cos(bodyAngle);

        float enableRise = (fabs(dx)>0.5 || fabs(dy)>0.5 || fabs(turn)>5) ? 1 : 0;

        // This is the x,y,z order in the referencial of the leg
        x = r + vx;
        y = vy;
        z = h + rise.getMod(legPhase)*alt*enableRise;
        if (i < 2) z += frontH;
    
        // Computing inverse kinematics
        if (computeIK(x, y, z, &a, &b, &c, L1, L2, backLegs ? L3_2 : L3_1)) {
            if (group) {
                a += crab*(-smoothBack);
            } else {
                a -= crab*(-smoothBack);
            }

            l1[i] = signs[0]*smoothBack*(a + step.getMod(legPhase)*turn);
            l2[i] = signs[1]*smoothBack*(b);
            l3[i] = signs[2]*smoothBack*(c - 180*smoothBackLegs);
        }

        if (i == 0) {
            Ax = x;
            Ay = y;
            Az = z;
        }
    }
}

void locomotion_set_dx(float dx_)
{
    dx = dx_;
}

void locomotion_set_dy(float dy_)
{
    dy = dy_;
}

void locomotion_set_turn(float turn_)
{
    turn = turn_;
}

#ifdef __EMSCRIPTEN__
using namespace emscripten;

EMSCRIPTEN_BINDINGS(locomotion) {
    function("locomotion_set_dx", &locomotion_set_dx);
    function("locomotion_set_dy", &locomotion_set_dy);
    function("locomotion_set_turn", &locomotion_set_turn);
    function("locomotion_init", &locomotion_init);
    function("locomotion_tick", &locomotion_tick);
    function("locomotion_get_motor", &locomotion_get_motor);
}
#endif

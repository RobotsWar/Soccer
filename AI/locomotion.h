#ifndef _LOCOMOTION_H
#define _LOCOMOTION_H

// Output angles
extern float l1[4];
extern float l2[4];
extern float l3[4];

void locomotion_init();
void locomotion_tick(float t);

void locomotion_set_dx(float dx);
void locomotion_set_dy(float dy);
void locomotion_set_turn(float turn);

float locomotion_get_motor(int idx);

#endif

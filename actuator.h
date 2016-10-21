#ifndef __ACTUATOR_H
#define __ACTUATOR_H

void initActuator();

void idleActuator();

void disarm();

void foldMast();

void unfoldMast();

void wheel(int value, char direction);

void thrust(int value);

void prerotate();

void rotate();

void elevon(int, int);

#endif

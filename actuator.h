#ifndef __ACTUATOR_H
#define __ACTUATOR_H

void initActuator();

void idleActuator();

void foldMast();

void unfoldMast();

void wheel(int value, char direction);

void thrust(int value);

void prerotate(int value);

#endif

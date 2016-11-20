#include <Arduino.h>
#include <Servo.h>

#define MAST1 0
#define MAST2 1
#define PREROTATOR 2
#define WHEEL 3
#define THRUST 4

#define ACTUATOR_NUM 5

char pins[ACTUATOR_NUM] = {2, 3, 4, 5, 6};

Servo actuators[ACTUATOR_NUM];

#define MIN 1000
#define MIDDLE 1500
#define MAX 2000

void idleActuator();

void initActuator() {
  for(int i=0; i<ACTUATOR_NUM; i++) {
    pinMode(pins[i], OUTPUT);
    Servo servo;
    actuators[i] = servo;
    actuators[i].attach(pins[i]);
  }

  idleActuator();
}

void idleActuator() {
  actuators[WHEEL].writeMicroseconds(MIDDLE);
  actuators[THRUST].writeMicroseconds(MIN);
  actuators[PREROTATOR].writeMicroseconds(MIN);
}

void foldMast() {
  actuators[MAST1].writeMicroseconds(MIN);
  actuators[MAST2].writeMicroseconds(MAX);
}

void unfoldMast() {
  actuators[MAST1].writeMicroseconds(MAX);
  actuators[MAST2].writeMicroseconds(MIN);
}

// direction: 0 (idle), 1 (normal), -1 (reverse)
void wheel(int value, char direction) {
  const int THRESHOLD = 1100; // motor doesn't rotate below 1100
  int pwm;
  if (value < THRESHOLD) pwm = 1500;
  else pwm = 1500 + (value - 1000) / 2.0 * direction;
  pwm = constrain(pwm, MIN, MAX);
  actuators[WHEEL].writeMicroseconds(pwm);
}

void thrust(int value) {
  int pwm = constrain(value, MIN, MAX);
  actuators[THRUST].writeMicroseconds(pwm);
}

void prerotate(int value) {
  value = (value - 1000) * 0.7 + 1000;
  int pwm = constrain(value, MIN, 1700);
  actuators[PREROTATOR].writeMicroseconds(pwm);
}

#include <Arduino.h>
#include <Servo.h>

#define MAST1 0
#define MAST2 1
#define PREROTATOR 2
#define WHEEL 3
#define THRUST 4
#define STOPPER 5
#define ELEVON_RIGHT 6
#define ELEVON_LEFT 7

#define ACTUATOR_NUM 8

char pins[ACTUATOR_NUM] = {2, 3, 4, 5, 6, 7, 8, 9};

Servo actuators[ACTUATOR_NUM];

#define MIN 1000
#define MIDDLE 1500
#define MAX 2000

char is_armed = 0;

void idleActuator();

void initActuator() {
  if (!is_armed) {
    for(int i=0; i<ACTUATOR_NUM; i++) {
      pinMode(pins[i], OUTPUT);
      Servo servo;
      actuators[i] = servo;
      actuators[i].attach(pins[i]);
    }
    is_armed = 1;

    idleActuator();
  }
}

void idleActuator() {
  actuators[WHEEL].writeMicroseconds(MIDDLE);
  actuators[THRUST].writeMicroseconds(MIN);
  actuators[PREROTATOR].writeMicroseconds(MIN);
  actuators[ELEVON_RIGHT].writeMicroseconds(MIDDLE);
  actuators[ELEVON_LEFT].writeMicroseconds(MIDDLE);
}

void disarm() {
  if (is_armed) {
    for(int i=0; i<ACTUATOR_NUM; i++) {
      actuators[i].detach();
    }
  }
}

void foldMast() {
  actuators[ELEVON_RIGHT].writeMicroseconds(MAX);
  actuators[ELEVON_LEFT].writeMicroseconds(MIN);

  actuators[MAST1].writeMicroseconds(MIN);
  actuators[MAST2].writeMicroseconds(MAX);
  //delay(4000);

  //actuators[ELEVON_RIGHT].writeMicroseconds(MIDDLE);
  //actuators[ELEVON_LEFT].writeMicroseconds(MIDDLE);
}

void unfoldMast() {
  actuators[ELEVON_RIGHT].writeMicroseconds(MAX);
  actuators[ELEVON_LEFT].writeMicroseconds(MIN);

  actuators[MAST1].writeMicroseconds(MAX);
  actuators[MAST2].writeMicroseconds(MIN);
  //delay(4000);

  //actuators[ELEVON_RIGHT].writeMicroseconds(MIDDLE);
  //actuators[ELEVON_LEFT].writeMicroseconds(MIDDLE);
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
  int pwm = value;
  pwm = constrain(pwm, MIN, MAX);
  actuators[THRUST].writeMicroseconds(pwm);
}

void prerotate() {
  //int rpm = getRPM();
}

void rotate() {
  int pwm, rpm;
  //rpm = getRPM();
  actuators[PREROTATOR].writeMicroseconds(pwm);
}

void elevon(int value1, int value2) {
  int pwm1 = constrain(value1, MIN, MAX);
  int pwm2 = constrain(value2, MIN, MAX);
  actuators[ELEVON_RIGHT].writeMicroseconds(pwm1);
  actuators[ELEVON_LEFT].writeMicroseconds(pwm2);
}

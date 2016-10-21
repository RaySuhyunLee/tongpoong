#include "receiver.h"
#include "actuator.h"
#include <Ultrasonic.h>

#define STATE_CAR 0x0
#define STATE_PLANE 0x1
#define STATE_FLIGHT 0x2
int state = STATE_CAR;

const unsigned long UPDATE_PERIOD = 20; // millis

#define RECV_CH_NUM 5
#define RECV_CH_THROTTLE 0
#define RECV_CH_AILERON 1
#define RECV_CH_ELEVATOR 2
#define RECV_CH_MODE 3
#define RECV_CH_REVERSE 4
int receiver[RECV_CH_NUM];

//#define DEBUG_RECV
//#define DEBUG_STATE

#define SONAR_TRIG 12
#define SONAR_ECHO 11
Ultrasonic sonar(SONAR_TRIG, SONAR_ECHO);

void setup() {
  initReceiver();

  Serial.begin(9600);
}

void loop() {
  static unsigned long lastUpdate;
  if (millis() - lastUpdate > UPDATE_PERIOD) {
    lastUpdate = millis();

    int temp = readReceiver(receiver, RECV_CH_NUM);
    if (temp != 0) { // signal loss. disarm
#ifdef DEBUG_RECV
      Serial.println("No signal");
      disarm();
#endif
    } else {
#ifdef DEBUG_RECV
      Serial.print(receiver[0], DEC);
      Serial.print(" ");
      Serial.print(receiver[1], DEC);
      Serial.print(" ");
      Serial.println(receiver[2], DEC);
#endif

#ifdef DEBUG_STATE
      Serial.println(state, DEC);
#endif
      initActuator();
   
      int throttle = receiver[RECV_CH_THROTTLE];
      if (throttle < 1100) {
        int mode = receiver[RECV_CH_MODE];
        if (mode <= 1330) { // car
          if (state == STATE_PLANE && sonar.Ranging(CM) < 40) {
            car();
          } else if (state == STATE_FLIGHT) {
            plane();
          }
        } else if (mode > 1330 && mode <= 1660) { // plane
          if (state == STATE_CAR || state == STATE_FLIGHT) {
            plane();
          }
        } else if (mode > 1660) { // flight
          if (state == STATE_CAR) {
            plane();
          } else if (state == STATE_PLANE) {
            flight();
          }
        }
      }
    }
  }

  process();
}

void car(void) {
  idleActuator();
  foldMast();
  // fold rotor
  state = STATE_CAR;
}

void plane(void) {
  idleActuator();
  unfoldMast();
  state = STATE_PLANE;
}

void flight(void) {
  idleActuator();
  prerotate();
}

void process(void) {
  char reverse;
  int distance;
  switch(state) {
  case STATE_CAR:
    reverse = receiver[RECV_CH_REVERSE] > 1500 ? -1 : 1;
    /*
    Serial.print(reverse, DEC);
    Serial.print(" ");
    Serial.println(receiver[RECV_CH_THROTTLE], DEC);*/
    wheel(receiver[RECV_CH_THROTTLE], reverse);
    break;
  case STATE_PLANE:
    distance = sonar.Ranging(CM);
    if (distance > 40) {
      wheel(0, 0);
    } else {
      wheel(receiver[RECV_CH_THROTTLE], 1); // no reverse
    }
    thrust(receiver[RECV_CH_THROTTLE]);
    elevon(receiver[RECV_CH_AILERON], receiver[RECV_CH_ELEVATOR]);

    //Serial.println(distance);
    break;
  case STATE_FLIGHT:
    break;
  }
}

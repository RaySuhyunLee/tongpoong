#include "receiver.h"
#include "actuator.h"

#define STATE_CAR 0x0
#define STATE_PLANE 0x1
int state = STATE_CAR;

const unsigned long UPDATE_PERIOD = 20; // millis

#define RECV_CH_NUM 4
#define RECV_CH_THROTTLE 0
#define RECV_CH_MODE 1
#define RECV_CH_REVERSE 2
#define RECV_CH_PREROTATOR 3

int receiver[RECV_CH_NUM];

//#define DEBUG_RECV
//#define DEBUG_STATE

void setup() {
  initReceiver();
  initActuator();

  Serial.begin(9600);
}

void loop() {
  static unsigned long lastUpdate;
  if (millis() - lastUpdate > UPDATE_PERIOD) {
    lastUpdate = millis();

    readReceiver(receiver, RECV_CH_NUM);
#ifdef DEBUG_RECV
    Serial.print(receiver[0], DEC);
    Serial.print(" ");
    Serial.print(receiver[1], DEC);
    Serial.print(" ");
    Serial.print(receiver[2], DEC);
    Serial.print(" ");
    Serial.println(receiver[3], DEC);
#endif

#ifdef DEBUG_STATE
    Serial.println(state, DEC);
#endif
   
    int throttle = receiver[RECV_CH_THROTTLE];
    int mode = receiver[RECV_CH_MODE];
    if (throttle < 1100) {
      if (mode <= 1500 && state == STATE_PLANE) { // car
        car();
      } else if (mode > 1500 && state == STATE_CAR) { // plane
        plane();
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

void process(void) {
  char reverse;

  Serial.println(state, DEC);
  switch(state) {
  case STATE_CAR:
    if (receiver[RECV_CH_THROTTLE] > 1000) {
      reverse = receiver[RECV_CH_REVERSE] > 1500 ? -1 : 1;
      wheel(receiver[RECV_CH_THROTTLE], reverse);
    } else {
      wheel(1000, 0);
    }
    break;
  case STATE_PLANE:
    thrust(receiver[RECV_CH_THROTTLE]);
    prerotate(receiver[RECV_CH_PREROTATOR]);
    //wheel(receiver[RECV_CH_THROTTLE], 1);
    break;
  }
}

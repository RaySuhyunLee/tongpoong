/* Receiver.cpp
 * 
 * read pwm signals from the RC receiver. 
 */

#include <Arduino.h>
#include <EnableInterrupt.h>

#define CHANNEL_MAX 4
int value[CHANNEL_MAX];

#define PIN_1 A1
#define PIN_2 A2
#define PIN_3 A3
#define PIN_4 A4

volatile long timeStamp[CHANNEL_MAX];
volatile unsigned long lastUpdate;

#define READ_SIGNAL(pin, channel) \
  if (digitalRead((pin)) == HIGH) { \
    timeStamp[(channel-1)] = micros(); \
  } else { \
    value[(channel-1)] = micros() - timeStamp[(channel-1)]; \
  } \
  lastUpdate = millis();

void readCh1() { READ_SIGNAL(PIN_1, 1) }
void readCh2() { READ_SIGNAL(PIN_2, 2) }
void readCh3() { READ_SIGNAL(PIN_3, 3) }
void readCh4() { READ_SIGNAL(PIN_4, 4) }

void initReceiver(void) {
  pinMode(PIN_1, INPUT);
  pinMode(PIN_2, INPUT);
  pinMode(PIN_3, INPUT);
  pinMode(PIN_4, INPUT);
  enableInterrupt(PIN_1, &readCh1, CHANGE);
  enableInterrupt(PIN_2, &readCh2, CHANGE);
  enableInterrupt(PIN_3, &readCh3, CHANGE);
  enableInterrupt(PIN_4, &readCh4, CHANGE);
}

int readReceiver(int *data, int length) {
  if (millis() - lastUpdate > 2000) { // signal loss
    return -1;
  } else {
    for (int i=0; i<length && i<CHANNEL_MAX; i++) {
      data[i] = value[i];
    }
    return 0;
  }
}

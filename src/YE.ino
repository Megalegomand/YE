#include "Arduino.h"

// ToDo List

int led_lowBat = 7;
int led_access = 6;
int led_denied = 5;
int sol = 8;

int testButton = 9;

enum NFC {ACCESS, DENIED, STANDBY};

NFC cs = STANDBY;

unsigned long timer = 0;
unsigned long blinkTime = 0;

int openTime = 5000;
int closeTime = 1500;
int blink = 75;

void setup() {
  pinMode(led_lowBat, OUTPUT);
  pinMode(led_access, OUTPUT);
  pinMode(led_denied, OUTPUT);
  pinMode(sol, OUTPUT);
  pinMode(testButton, INPUT);
}

void loop() {
  if (cs == STANDBY) {
    cs = checkNFC();
    switch (cs) {
      case ACCESS: {
        digitalWrite(led_access, HIGH);
        digitalWrite(sol, HIGH);
        timer = millis(); }
        break;
      case DENIED: {
        digitalWrite(led_denied, HIGH);
        timer = millis(); }
        blinkTime = millis();
        break;
      default:
        break;
    }
  } else {
    switch (cs) {
      case ACCESS:
        if (timer + openTime <= millis()) {
          digitalWrite(led_access, LOW);
          digitalWrite(sol, LOW);
          cs = STANDBY;
        }
        break;
      case DENIED:
        if (blinkTime + blink > millis()) {
          digitalWrite(led_denied, HIGH);
        } else {
          digitalWrite(led_denied, LOW);
        }
        if (blinkTime + 2*blink <= millis()) {
          blinkTime = millis();
        }
        if (timer + closeTime <= millis()) {
          digitalWrite(led_denied, LOW);
          cs = STANDBY;
        }
        break;
      default:
        break;
    }
  }
}

NFC checkNFC() {
  if (digitalRead(testButton)) {
    if (random(2) == 1) {
      return ACCESS;
    } else {
      return DENIED;
    }
  }
  return STANDBY;
}

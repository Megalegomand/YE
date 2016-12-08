#include "Arduino.h"

// ToDo List

int led_lowBat = 7;
int led_access = 6;
int led_denied = 5;

int testButton = 9;

enum NFC {ACCESS, DENIED, LOLNOPE};

void setup() {
  pinMode(led_lowBat, OUTPUT);
  pinMode(led_access, OUTPUT);
  pinMode(led_denied, OUTPUT);
  pinMode(testButton, INPUT);
}

void loop() {
  
}

void NFC checkNFC() {
  return NFC.ACCESS;
}

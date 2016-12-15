#include "Arduino.h"

  #include <SPI.h>
                  // Arduino 5V (VCC)  to TFT: LED, VCC
                  // Arduino GND to TFT: GND
                  // Arduino D13 (SCK) to TFT: SCK
                  // Arduino D11 (MOSI) to TFT:
  #define cs  10  // Arduino D10 (SS) to TFT: CS
  #define dc   9  // Arduino D9 to TFT: A0
  #define rst  8  // Arduino-D8 to TFT: RESET

#define gpo_pin 7 // Arduino pin 7 GPO
M24SR m24sr(gpo_pin);

int led_lowBat = 5;
int led_access = 4;
int led_denied = 6;
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

  Serial.begin(9600);
    //for debug purpose
    //m24sr._verbose = true;
    //m24sr._cmds = true;
    m24sr._setup();

  m24sr.displaySystemFile();
}

void loop() {
  if (m24sr.checkGPOTrigger()) {
        displayNDefRecord();
  }
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

void displayNDefRecord() {
    //read NDef message from memory
    NdefMessage* pNDefMsg = m24sr.getNdefMessage();
    if (pNDefMsg != NULL) {
       pNDefMsg->print();
       NdefRecord rec = pNDefMsg->getRecord(0);
       String txt = rec.toString();
       Serial.print(F("NDefRecord: "));
       Serial.println(txt);
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

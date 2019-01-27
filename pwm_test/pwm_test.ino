#include <Arduino.h>
#include <Wire.h>
#include <cmath>
#include "chemrap.h"

#define SERIAL_BAUD_RATE 115200

Chemrap chemrap = Chemrap();

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println("Uh...tryn");
    chemrap.begin();
    Serial.println("maybe?");
    delay(10);
}

void loop() {

}


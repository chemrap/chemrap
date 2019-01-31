#include <Arduino.h>
#include <Wire.h>
#include <cmath>
#include "chemrap.h"

#define SERIAL_BAUD_RATE 115200
#define SDA 32
#define SCL 33

Chemrap chemrap = Chemrap();
    
void setup() {
    Serial.begin(115200);
    chemrapSetup();
}

void loop() {
    chemrapTest();
}

void chemrapSetup() {
    Serial.begin(SERIAL_BAUD_RATE);
    chemrap.begin();
    delay(10);
    chemrap.info();
}

void chemrapTest() {

    // cycle through full turn for 4 motors, ports 12-15 on the pwm board
    for(int angle=10; angle<=170; angle++) {
        Serial.print(angle);Serial.println("  ");
        chemrap.setAngle(15, angle);
        chemrap.setAngle(14, angle);
        chemrap.setAngle(13, angle);
        chemrap.setAngle(12, angle);
        delay(100);
    }
}

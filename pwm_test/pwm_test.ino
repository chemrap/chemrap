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
    // chemrapSetup();
    servoDriverSetup();
}

void loop() {
    // chemrapTest();
    servoDriverTest();
}


void servoDriverSetup() {
    Wire.begin(SDA, SCL);
    delay(10);
    writeByte(0x00, 0x80);
    delay(50);
    uint8_t mode1 = readByte(0x00);
    uint8_t sleep = (mode1 & 0x7F) | 0x10;
    writeByte(0x00, sleep);
    writeByte(0xFE, 101);  // 101
    writeByte(0x00, mode1);

}

void servoDriverTest() {
    uint8_t onLow=0x42;
    uint8_t onHigh=0x43;
    uint8_t offLow=0x44;
    uint8_t offHigh=0x45;

    // writeByte(onLow, 0x96);
    // writeByte(onHigh, 0x00);
    // writeByte(offLow, 0xFF);
    // writeByte(offHigh, 0x01);

    for (uint16_t low=0x0096; low<=0x02BC; low++) {
        Serial.println(low);
        writeByte(onLow, 0x96);
        writeByte(onHigh, 0x00);
        writeByte(offLow, low);
        writeByte(offHigh, low>>8);
        delay(50);
    }

}


void chemrapSetup() {
    Serial.begin(SERIAL_BAUD_RATE);
    chemrap.begin();
    delay(10);
    chemrap.info();
}

void chemrapTest() {
    int motor = 15;

    for(int angle=0; angle<=180; angle++) {
        Serial.print(angle);Serial.print("  ");
        chemrap.setAngle(motor, angle);
        delay(250);
    }
}

void writeByte(uint8_t addr, uint8_t val) {
    Wire.beginTransmission(ADDRESS);
    Wire.write(addr);
    Wire.write(val);
    Wire.endTransmission();
}

uint8_t readByte(uint8_t addr) {
    Wire.beginTransmission(ADDRESS);
    Wire.write(addr);
    Wire.endTransmission();

    Wire.requestFrom(ADDRESS, 1);
    return Wire.read();
}

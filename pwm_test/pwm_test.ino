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
    // servoDriverSetup();
}

void loop() {
    chemrapTest();
    // servoDriverTest();
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
    delay(1000);
}

void servoDriverTest() {
    uint8_t onLow=0x42;
    uint8_t onHigh=0x43;
    uint8_t offLow=0x44;
    uint8_t offHigh=0x45;

    for (uint16_t low=0x02BC; low>0x0000; --low) {
        Serial.print("Low");    Serial.print(" ");
        Serial.print((uint8_t)(low>>8), HEX);    Serial.print(" ");
        Serial.print((uint8_t)low, HEX);         Serial.print(" ");
        Serial.print((uint8_t)(0x02BC>>8), HEX); Serial.print(" ");
        Serial.print((uint8_t)0x02BC, HEX);      Serial.println(" ");
        writeByte(onHigh,   (uint8_t)(low>>8));
        writeByte(onLow,    (uint8_t)low);
        writeByte(offHigh,  (uint8_t)(0x02BC>>8));
        writeByte(offLow,   (uint8_t)0x02BC);
        delay(50);
    }

    for (uint16_t high=0x001D; high<=0x02BC; high++) {
        Serial.print("Low");    Serial.print(" ");
        Serial.print((uint8_t)(0x001D>>8), HEX);    Serial.print(" ");
        Serial.print((uint8_t)0x001D, HEX);         Serial.print(" ");
        Serial.print((uint8_t)(high>>8), HEX); Serial.print(" ");
        Serial.print((uint8_t)high, HEX);      Serial.println(" ");
        writeByte(onLow,    0x1D);
        writeByte(onHigh,   0x00);
        writeByte(offLow,   high);
        writeByte(offHigh,  high>>8);
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

    for(int angle=10; angle<=170; angle++) {
        Serial.print(angle);Serial.println("  ");
        chemrap.setAngle(15, angle);
        chemrap.setAngle(14, angle);
        chemrap.setAngle(13, angle);
        chemrap.setAngle(12, angle);
        delay(100);
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

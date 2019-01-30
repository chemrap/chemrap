#include "chemrap.h"
#include <Wire.h>
#include <cmath>


Chemrap::Chemrap() {
    i2cBus = &Wire;
}

void Chemrap::begin(void) {
    
    i2cBus->begin(SDA, SCL);
    delay(50);
    writeByteTo(MODE1, 0x80);
    setPrescale(60); // default frequency
}

void Chemrap::reset(void) {

    writeByteTo(MODE1, 0x80);
    delay(10);
}

void Chemrap::setAngle(int motor, float angle) {

    uint8_t motorReg    = SERVO_REG_BASE + 4*motor;
    uint16_t on         = SERVO_MIN;
    uint16_t off        = angle/180 * (SERVO_MAX-SERVO_MIN) + on + MIN_PULSE_WIDTH;

    setMotorPwm(motorReg, on, off);
}

void Chemrap::setPrescale(float freq) {

    frequency = freq;
    float internalClockFreq = 25000000;

    uint16_t scaling    = round(internalClockFreq / (4096 * freq)) - 1;
    uint8_t mode1       = readByteFrom(MODE1);
    uint8_t sleepMode   = (mode1 & 0x7F) | 0x10;

    writeByteTo(MODE1, sleepMode);
    writeByteTo(PRESCALE, scaling);
    writeByteTo(MODE1, mode1);

    delay(10);
}

void Chemrap::writeByteTo(uint8_t reg, uint8_t val) {

    i2cBus->beginTransmission(ADDRESS);
    i2cBus->write(reg);
    i2cBus->write(val);
    i2cBus->endTransmission();
}

void Chemrap::setMotorPwm(uint8_t motorReg, uint16_t on, uint16_t off) {

    // clean this up
    uint8_t onLow   = (uint8_t)on;
    uint8_t onHigh  = (uint8_t)(on>>8);
    uint8_t offLow  = (uint8_t)off;
    uint8_t offHigh = (uint8_t)(off>>8);

    uint8_t mode1 = readByteFrom(MODE1);
    
    writeByteTo(MODE1, mode1 | 0xA0); // turn on autoincrement

    i2cBus->beginTransmission(ADDRESS);
    i2cBus->write(motorReg);
    i2cBus->write(onLow);
    i2cBus->write(onHigh);
    i2cBus->write(offLow);
    i2cBus->write(offHigh);
    i2cBus->endTransmission();

    writeByteTo(MODE1, mode1 | 0x80);  // turn off autoincrement

}

uint8_t Chemrap::readByteFrom(uint8_t reg) {

    i2cBus->beginTransmission(ADDRESS);
    i2cBus->write(reg);
    i2cBus->endTransmission();
    i2cBus->requestFrom(ADDRESS, 1);

    return i2cBus->read();
}

void Chemrap::info(void) {
    Serial.println("----------------------------------");
    Serial.print("MODE1: ");Serial.println(readByteFrom(MODE1));
    Serial.print("PRESCALE: ");Serial.println(readByteFrom(PRESCALE));
    Serial.println("----------------------------------");
}


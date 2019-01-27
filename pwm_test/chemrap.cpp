#include "chemrap.h"
#include <Wire.h>
#include <cmath>


Chemrap::Chemrap() {
    i2cBus = &Wire;
}


void Chemrap::begin(void) {
    Serial.println("starting i2c");
    i2cBus->begin(SCL, SDA);
    Serial.println("prescale");
    setPrescale(60); // default frequency
}

void Chemrap::reset(void) {
    writeByteTo(MODE1, 0x80);
    delay(10);
}

void Chemrap::setAngle(int motor, float angle) {
    uint8_t motorReg = SERVO_REG_BASE + 4*motor;
    uint16_t on = SERVO_MIN;
    uint16_t off = angle/180 * (SERVO_MAX-SERVO_MIN) + on;

    setMotorPwm(motorReg, on, off);
}

void Chemrap::setPrescale(float freq) {
    frequency = freq;

    float internalClockFreq = 25000000;
    uint16_t scaling = round(internalClockFreq / (4096 * freq)) - 1;

    uint8_t mode1Reg = readByteFrom(MODE1);
    uint8_t sleepMode = (mode1Reg & 0x7F) | 0x10;
    writeByteTo(MODE1, sleepMode);
    writeByteTo(PRESCALE, scaling);
    writeByteTo(MODE1, mode1Reg);
    delay(10);
}

void Chemrap::writeByteTo(uint8_t reg, uint8_t val) {
    i2cBus->beginTransmission(ADDRESS);
    i2cBus->write(ADDRESS);
    i2cBus->write(val);
    i2cBus->endTransmission();
}

void Chemrap::setMotorPwm(uint8_t motorReg, uint16_t on, uint16_t off) {

    i2cBus->beginTransmission(ADDRESS);
    i2cBus->write(motorReg);
    i2cBus->write(on);
    i2cBus->write(motorReg+1);
    i2cBus->write(on>>8);
    i2cBus->write(motorReg+2);
    i2cBus->write(off);
    i2cBus->write(motorReg+3);
    i2cBus->write(off>>8);
    i2cBus->endTransmission();
}

uint8_t Chemrap::readByteFrom(uint8_t reg) {
    i2cBus->beginTransmission(ADDRESS);
    i2cBus->write(reg);
    i2cBus->endTransmission();

    i2cBus->requestFrom(ADDRESS, 1);
    return i2cBus->read();
}

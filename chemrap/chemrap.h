#ifndef __CHEMRAP_H
#define __CHEMRAP_H

#include <Arduino.h>
#include <Wire.h>

#define MODE1 0x0
#define PRESCALE 0xFE
#define SERVO_REG_BASE 0x06
#define SDA 32
#define SCL 33
#define ADDRESS 0x44
#define MIN_PULSE_WIDTH 150
#define SERVO_MIN 32
#define SERVO_MAX 700 - MIN_PULSE_WIDTH

class Chemrap {
    public:
        Chemrap();
        void begin(void);
        void reset(void);
        void calibrateMotor(int motor);
        void setAngle(int motor, float angle);
        void info(void);
    private:
        void setPrescale(float freq);
        void setMotorPwm(uint8_t motorReg, uint16_t on, uint16_t off);
        void writeByteTo(uint8_t reg, uint8_t val);
        uint8_t readByteFrom(uint8_t reg);
        TwoWire *i2cBus;
        float frequency;
};
#endif

#include "chemrap.h"
#include <Wire.h>
#include <cmath>


//! Constructor, assigns the TWI bus used to the object.
Chemrap::Chemrap() {
    i2cBus = &Wire;
}

//! initializes the is2 bus, starts the pwm driver, and sets the default frequency
void Chemrap::begin(void) {
    
    i2cBus->begin(SDA, SCL);
    delay(50);
    writeByteTo(MODE1, 0x80);
    setPrescale(60); // default frequency for servos ~ 60Hz
}

//! resets the pwm driver and turns it on
void Chemrap::reset(void) {

    writeByteTo(MODE1, 0x80);
    delay(10);
}

/*!
 Sets the angle for a specific motor attached to the pwm driver.
 The motorReg register is the first of 4 registered associated with setting the 
 12 bit DAC values.  Two 8 bit registers for the "on" pulse start and two
 for the "off" pulse end.  The motor registers start at address SERVO_REG_BASE.

 The servos are calibrated with a SERVO_MIN and SERVO_MAX value which determine the 
 values between which a speciifc motor will turn.  While these values are relative to
 each other, absolute numbers are chosen during calibration starting with 0.

 The motors will not turn wihout a minimum pulse width, and there is a risk of
 lockup witout it.  When the pulse width is calculated, this is added on regardless
 of value.

 The start registser and pulse location are passed to setMotorPwm.

 \sa setMotorPwm
 \param motor an integer representing a port on the pwm chip, 0-15
 \param angle a float in 0.0-180.0
 */
void Chemrap::setAngle(int motor, float angle) {

    uint8_t motorReg    = SERVO_REG_BASE + 4*motor;
    uint16_t on         = SERVO_MIN;
    uint16_t off        = angle/180 * (SERVO_MAX-SERVO_MIN) + on + MIN_PULSE_WIDTH;

    setMotorPwm(motorReg, on, off);
}

//! Sets the prescale value for the pwm driver.  See datasheet.
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

/*! 
 Sets the PWM pulse width for a motor corresponding to the 4 registers beginning at motorReg.
 This function uses autoincrement to write 4 bytes in sequence to the registers for a motor.
 See the datasheet for the PWM controller.

 \param motorReg the start of 4 sequential registers holding the start and end pulse values
 \param on a 16bit value which holds the 12bit DAC start value for the pwm pulse
 \param on a 16bit value which holds the 12bit DAC end value for the pwm pulse
*/
void Chemrap::setMotorPwm(uint8_t motorReg, uint16_t on, uint16_t off) {

    // clean this up
    uint8_t onLow   = (uint8_t)on;
    uint8_t onHigh  = (uint8_t)(on>>8);
    uint8_t offLow  = (uint8_t)off;
    uint8_t offHigh = (uint8_t)(off>>8);

    uint8_t mode1 = readByteFrom(MODE1);    //< retrieve and store MODE1 register values
    
    writeByteTo(MODE1, mode1 | 0xA0);       //< turn on autoincrement

    i2cBus->beginTransmission(ADDRESS);
    i2cBus->write(motorReg);
    i2cBus->write(onLow);
    i2cBus->write(onHigh);
    i2cBus->write(offLow);
    i2cBus->write(offHigh);
    i2cBus->endTransmission();

    writeByteTo(MODE1, mode1 | 0x80);       //< restore MODE1 and turn off autoincrement

}

//! helper function to read 1 byte only from a register
uint8_t Chemrap::readByteFrom(uint8_t reg) {

    i2cBus->beginTransmission(ADDRESS);
    i2cBus->write(reg);
    i2cBus->endTransmission();
    i2cBus->requestFrom(ADDRESS, 1);

    return i2cBus->read();
}

//! Helper function to write 1 byte only to a register
void Chemrap::writeByteTo(uint8_t reg, uint8_t val) {

    i2cBus->beginTransmission(ADDRESS);
    i2cBus->write(reg);
    i2cBus->write(val);
    i2cBus->endTransmission();
}

void Chemrap::info(void) {
    Serial.println("----------------------------------");
    Serial.print("MODE1: ");Serial.println(readByteFrom(MODE1));
    Serial.print("PRESCALE: ");Serial.println(readByteFrom(PRESCALE));
    Serial.println("----------------------------------");
}


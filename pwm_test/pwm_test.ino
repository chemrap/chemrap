#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define SERIAL_BAUD_RATE 115200
#define SDA 32
#define SCL 33


// Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN 150
#define SERVOMAX 600


uint8_t servonum = 0;

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    Wire.begin(SDA, SCL);
    // pwm.begin(Wire);
    // pwm.setPWMFreq(60);
    delay(10);
}

void loop() {
    byte addr = 0x44; // 1100 100 = A4 (try 44 after) 
    byte resp = 0x06;

    Wire.beginTransmission(addr);
    Wire.write(0x00);
    Wire.endTransmission();

    resp = Wire.requestFrom(addr, 0x01);
    Serial.println(resp);
    Wire.endTransmission();

}


void find_addr() {
    // 1000 1000 = 0x88 = 136
    byte addr = 0x88; 
    uint8_t resp;

    for (uint8_t chip=0x00; chip<0xEF; chip++) {
        Wire.beginTransmission(chip);
        Wire.write(0x01); 
        // Wire.requestFrom(1, 1);
        Wire.endTransmission();
        resp = Wire.read();

        Serial.print("chip:"); Serial.print(chip, HEX); Serial.print(" resp:");
        Serial.print(resp, HEX); Serial.print(" avail: "); Serial.println(Wire.available());

        Wire.flush();
        delay(100);
    }
} 

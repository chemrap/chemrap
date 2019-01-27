#include "ServoController.h"
#include <Arduino.h>

void ServoController::ServoController() {}

void addMotor(std::String name, int pin) {

    std::map<std::String, Servo>::iterator it;
    it = this.motors.find(name);

    // if name exists, overwrite it.
    if(it != this.motors.end())
        this.motors.erase(it);

    std::pair <int, Servo> motor(pin, Servo(pin));
    this.motors[name] = motor;
} 

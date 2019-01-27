#ifndef _SERVO_CONTROLLER_H
#define _SERVO_CONTROLLER_H

#include "Servo.h"
#include <map>
#include <std>


class ServoController{
    public:
        ServoController();
        void addMotor(String name, int pin);

    private:
        int num_motors=0;
        map<std::String, std::pair<int, Servo>> motors;

#endif

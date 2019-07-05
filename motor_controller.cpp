#include <Arduino.h>
#include <Motor.h>
#include "motor_controller.h"

motor_controller::motor_controller(int pin){
    pin_id = pin;
}

void motor_controller::stop(){
    SetMotor(pin_id, 0, 0);
}

void motor_controller::go(int speed){
    int new_speed = (int)(default_speed / 100.0 * speed);
    SetMotor(pin_id, new_speed, 1);
}

void motor_controller::go(){
    go(100);
}
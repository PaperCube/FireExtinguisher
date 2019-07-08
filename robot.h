#ifndef ROBOT_H
#define ROBOT_H

#include <Arduino.h>
#include <Servo.h>
#include "controllers.h"

int convert(int);

struct robot {
   private:
    bool is_setup;
    
    motor_controller *motor_l, *motor_r, *motor_f, *motor_b;
    motor_pair *mot_pair_lr, *mot_pair_fb;
    quad_directional *motor_group;
    Servo *arm;

   public:
    robot();
    
    void setup();
    void run();
    void move_until_blocked(direction, int);
};

#endif
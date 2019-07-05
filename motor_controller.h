#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

struct motor_controller{
    int pin_id;
    int default_speed = 120;
    
    motor_controller(int);
    void stop();
    void go(int speed);
    void go();
};

struct motor_pair{
    
};

#endif MOTOR_CONTROLLER_H
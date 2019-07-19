#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include "definitions.h"

extern direction reverse_direction(direction);

struct builtin_motor_driver {
  private:
    int digital_l, digital_r, analog_pin;
  public:
    bool all_low_stop;
    builtin_motor_driver(int);
    void go(int);
};

struct motor_controller {
  public:
    int pwm_pin;
    int direction_pin;
    int max_power = 0;

    motor_controller();
    motor_controller(int, int);
    void init(int, int);
    void stop();
    void go(int speed);
    void go();

  private:
};

struct motor_pair {
  public:
    motor_pair(motor_controller *, motor_controller *);
    void stop();
    void go(int);
    void go();
    // void turn(float);
  private:
    motor_controller *left, *right;
    int               current_speed;

  protected:
};

/*
 *
 *      -----------=========----------
 *      |           Front            |
 *      |                            |
 *      |                            |
 *     | |                          | |
 *     | | Left              Right  | |
 *     | |                          | |
 *      |                            |
 *      |            Back            |
 *      -----------=========----------
 *
 *
 *    Pin configuration (as for July 6th, 2019)
 *    PWM         Digital
 *    2 (IN 3)    26 (IN 4)   Right
 *    3 (IN 2)    27 (IN 1)   Left
 *    4 (IN 3)    28 (IN 4)   Back
 *    5 (IN 2)    29 (IN 1)   Front
 */
struct quad_directional {
  private:
    motor_pair *pair_lr, // pair_lr goes forward and backward
        *pair_fb;        // pair_fb goes leftward and rightward
    double    multiplier_lr, multiplier_fb;
    direction current_direction;
    int       current_speed = 0;

  public:
    quad_directional(motor_pair *, direction, motor_pair *, direction);
    void stop();
    void set_direction(direction);
    void go();
    void go(int);
    void reverse_and_stop(int);
    void reverse_and_stop(int, int);
};

struct arm {
  private:
    int pin;
    int current_angle;

  public:
    arm(int);
    void rotate_by(int);
    void rotate_to(int);
};

#endif
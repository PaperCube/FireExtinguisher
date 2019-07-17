#ifndef ROBOT_H
#define ROBOT_H

#include "controllers.h"
#include "parameters.h"
#include "sensors.h"
#include <Arduino.h>

int convert(int);

struct robot {
  private:
    bool is_setup;

    motor_controller *   motor_l, *motor_r, *motor_f, *motor_b;
    motor_pair *         mot_pair_lr, *mot_pair_fb;
    sensor_pair *        sensor[4];
    builtin_motor_driver mechanic_arm_motor;

    void init_sensors();
    void init_motors();

  public:
    quad_directional *motor_group;

    robot();
    void set_max_speed(int);
    void set_max_speed(int, int, int, int);
    void setup();
    void run();
    void move_until_blocked(direction, int = 0);

    /* Begin: delegated by quad_directional *motor_group */
    void stop();
    void set_direction(direction);
    void go();
    void go(int);
    void reverse_and_stop(int);
    void reverse_and_stop(int, int);
    /* End: delegate */

    int read_sensor(direction);
    int read_sensor(direction, signed char);

    int read_sensor_raw(direction, signed char = 0);

    void start_arm();
    void stop_arm();
};

#endif
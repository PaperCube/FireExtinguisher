#ifndef ROBOT_H
#define ROBOT_H

#include "_robot_all_includes.h"

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

    motor_controller *get_motor(direction_t);
    void              set_max_speed(int);
    void              set_max_speed(int, int, int, int);
    void              set_max_power(int);
    void              set_max_power(int, int, int, int);
    void              reverse_power_ratio(float, float, float, float);
    void              setup();
    void              run();
    void              __test();
    void              __test_power();
    void              move_until_blocked(direction_t, int = 0);

    /* Begin: delegated by quad_directional *motor_group */
    void stop();
    void set_direction(direction_t);
    void go();
    void go(int);
    void reverse_and_stop(int);
    void reverse_and_stop(int, int);
    /* End: delegate */

    int read_sensor(direction_t);
    int read_sensor(direction_t, signed char);

    int read_sensor_raw(direction_t, signed char = 0);

    void start_arm();
    void stop_arm();

    void set_sensors(int *);
    void set_sensors(int, int, int, int, int, int, int, int);

    void rotate_at(int);
    void rotate_timed(int, int);
    void fix(direction_t);
};

#endif
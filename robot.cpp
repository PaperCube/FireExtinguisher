#include "robot.h"
#include "buttons.h"
#include "buzzer.h"
#include "controllers.h"
#include "display.h"
#include "math_util.h"
#include "sensors.h"
#include "sout.h"
#include <Arduino.h>
#include <Servo.h>

using namespace serial;

robot::robot() : mechanic_arm_motor(MECHANIC_ARM_MOTOR_ID) {
    this->is_setup = false;
}

void robot::init_sensors() { sensor_manager::instance.prepare(); }

void robot::init_motors() {
    // motor_controller *motor = new motor_controller;
    // motor->init(3, 27);
    // motor->max_speed = 100;
    // motor->go();
    motor_l = new motor_controller;
    motor_r = new motor_controller;
    motor_f = new motor_controller;
    motor_b = new motor_controller;
    motor_l->init(3, 27);
    motor_r->init(2, 26);
    motor_f->init(5, 29);
    motor_b->init(4, 28);

    set_max_speed(FULL_SPEED);

    mot_pair_lr = new motor_pair(motor_l, motor_r);
    mot_pair_fb = new motor_pair(motor_f, motor_b);

    motor_group = new quad_directional(mot_pair_lr, direction::FORWARD,
                                       mot_pair_fb, direction::RIGHTWARD);

    // motor_group->set_direction(direction::FORWARD);
    // motor_group->go();
    // while(true);
}

void robot::setup() {
    sdebug << "Preparing" << endl;
    buzz_patterns::prepare();
    buzzer buz;
    buz.buzz(buzz_patterns::DOUBLE_SHORT);
    button b(1);
    sdebug << "Done preparing" << endl;
    b.wait_until_released();
    sdebug << "Received singal: key 1 released" << endl;

    display::reset();
    display::update_display();
    sdebug << "Updating display" << endl;
    this->is_setup = true;
}

void print_visually(int v) {
    const int gap_size = 6;
    int       n        = math::round_d((double)v / gap_size);
    for (int i = 0; i < n; i++) {
        sdebug << "#";
    }
    sdebug << "  " << v << endl;
}

void robot::set_max_speed(int v) {
    motor_controller *const motors[] = {motor_l, motor_r, motor_f, motor_b};
    for (auto iter = motors; iter != motors + 4; iter++) {
        (*iter)->max_speed = v;
    }
}

void robot::set_max_speed(int vf, int vl, int vr, int vb) {
    motor_f->max_speed = vf;
    motor_l->max_speed = vl;
    motor_r->max_speed = vr;
    motor_b->max_speed = vb;
}

void robot::move_until_blocked(direction d, const int timeout) {
    button b(3);
    motor_group->set_direction(d);
    auto *sensor = sensor_manager::instance.sensor_at(d);
    motor_group->go();
    int            last_loop_time_stamp       = millis();
    unsigned long  last_stopping_request_time = 0;
    math::averager avger;
    while (true) {
        int val_converted = sensor->read();
        // sdebug << "Sensor : " << sensor_value << " | converting to "
        //        << convert(sensor_value) << endl;
        sdebug << "Sensor(conv): ";
        print_visually(val_converted);
        if (val_converted < STOP_THRESHOLD) {
            if (last_stopping_request_time == 0 && timeout > 0) {
                last_stopping_request_time = millis();
            } else if (timeout <= 0 ||
                       (long)(millis() - last_stopping_request_time) >
                           timeout) {
                sdebug << "Attempting to stop" << endl;
                motor_group->reverse_and_stop(32767, 100);
                break;
            }
        } else {
            if (val_converted < SLOW_THRESHOLD) {
                int       window_size = SLOW_THRESHOLD - STOP_THRESHOLD;
                int       dist        = val_converted - STOP_THRESHOLD;
                const int lowest      = 50;
                int       speed =
                    lowest + ((double)dist / window_size) * (100 - lowest);
                motor_group->go(speed);
            } else {
                motor_group->go();
            }
            last_stopping_request_time = 0;
        }

        unsigned long current_time_stamp = millis();
        avger.update(current_time_stamp - last_loop_time_stamp);
        last_loop_time_stamp = current_time_stamp;
    }
    display::params.loop_avg = avger.get_avg();
    display::update_display();
    sdebug << "Quiting loop." << endl;
}

void debug_prox_sensor() {
    while (true) {
        for (int i = 0; i < 4; i++) {
            if (i != 0) {
                sdebug << ", ";
            }
            sdebug << sensor_manager::instance.sensor_at((direction)i)->read();
        }
        sdebug << endl;
        delay(100);
    }
}

void robot::run() {
    if (!is_setup)
        return;
    button btn(2);
    while (true) {
        btn.wait_until_released();
        move_until_blocked(direction::FORWARD);
    }
    debug_prox_sensor();
    motor_group->go();
    const direction directions[] = {direction::FORWARD, direction::RIGHTWARD,
                                    direction::BACKWARD, direction::LEFTWARD};
    while (true) {
        for (auto d : directions) {
            sdebug << "Travelling to direction " << (int)d << endl;
            motor_group->set_direction(d);
            delay(1000);
            motor_group->stop();
            delay(200);
        }
    }
    delay(100000);
}

void robot::stop() { motor_group->stop(); }

void robot::set_direction(direction d) { motor_group->set_direction(d); }

void robot::go() { motor_group->go(); }

void robot::go(int v) { motor_group->go(v); }

void robot::reverse_and_stop(int v) { motor_group->reverse_and_stop(v); }

void robot::reverse_and_stop(int v, int v2) {
    motor_group->reverse_and_stop(v, v2);
}

int robot::read_sensor(direction d) {
    return sensor_manager::instance.sensor_at(d)->read();
}

int robot::read_sensor(direction d, signed char which) {
    auto *sensor = sensor_manager::instance.sensor_at(d);
    if (which < 0) {
        return sensor->read_l();
    }
    return sensor->read_r();
}

int robot::read_sensor_raw(direction d, signed char which) {
    auto *sensor = sensor_manager::instance.sensor_at(d);
    if(which == 0){
        return sensor->read_raw();
    } else if(which < 0){
        return sensor->get_left()->read_raw_calibrated();
    }
    return sensor->get_right()->read_raw_calibrated();
}

void robot::start_arm() { mechanic_arm_motor.go(MECHANIC_ARM_ROTATION_SPEED); }

void robot::stop_arm() { mechanic_arm_motor.go(0); }
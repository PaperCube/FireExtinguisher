#include "robot.h"
#include "buttons.h"
#include "controllers.h"
#include "display.h"
#include "sensors.h"
#include "sout.h"
#include <Arduino.h>
#include <Servo.h>

using namespace serial;

const int MOTOR_L        = 3;
const int MOTOR_R        = 4;
const int FULL_SPEED     = 120;
const int STOP_THRESHOLD = 40;
const int SLOW_THRESHOLD = 80;

robot::robot() {
    this->is_setup    = false;
    this->mot_pair_lr = false;
}

void robot::setup() {
    sdebug << "Preparing" << endl;
    prox_sensor::prepare_builtin_sensors();
    button b(1);
    b.wait_until_released();
    sdebug << "Received singal: key 1 released" << endl;
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

    motor_controller *const motors[] = {motor_l, motor_r, motor_f, motor_b};
    for (auto iter = motors; iter != motors + 4; iter++) {
        (*iter)->max_speed = FULL_SPEED;
    }

    mot_pair_lr = new motor_pair(motor_l, motor_r);
    mot_pair_fb = new motor_pair(motor_f, motor_b);

    motor_group =
        new quad_directional(mot_pair_lr, FORWARD, mot_pair_fb, RIGHTWARD);

    display::reset();
    display::update_display();
    sdebug << "Updating display" << endl;
    this->is_setup = true;
}

void robot::move_until_blocked(direction d, int timeout = 100) {
    motor_group->set_direction(d);
    prox_sensor *sensor = prox_sensor::sensor_at(d);
    motor_group->go();
    int           last_loop_time_stamp       = 0;
    unsigned long last_stopping_request_time = 0;
    while (true) {
        unsigned long current_time_stamp = millis();
        sdebug << "Last loop took "
               << (current_time_stamp - last_loop_time_stamp) << " ms." << endl;
        last_loop_time_stamp            = current_time_stamp;
        int val_converted               = sensor->read();
        display::sensor_value           = 0; // todo remove.
        display::sensor_value_converted = val_converted;
        display::update_display();
        // sdebug << "Sensor : " << sensor_value << " | converting to "
        //        << convert(sensor_value) << endl;

        if (val_converted < STOP_THRESHOLD) {
            if (last_stopping_request_time == 0) {
                last_stopping_request_time = millis();
            } else if (millis() - last_stopping_request_time > timeout) {
                mot_pair_lr->stop();
                break;
            }
        } else {
            if (val_converted < SLOW_THRESHOLD) {
                mot_pair_lr->go(40);
            } else {
                mot_pair_lr->go();
            }
            last_stopping_request_time = 0;
        }
    }
}

void debug_prox_sensor() {
    while (true) {
        for (int i = 0; i < 4; i++) {
            if (i != 0) {
                sdebug << ", ";
            }
            sdebug << prox_sensor::sensor_at(i)->read();
        }
        sdebug << endl;
        delay(100);
    }
}

void robot::run() {
    if (!is_setup)
        return;
    debug_prox_sensor();
    motor_group->go();
    const direction directions[] = {FORWARD, RIGHTWARD, BACKWARD, LEFTWARD};
    while (true) {
        for (auto d : directions) {
            sdebug << "Travelling to direction " << d << endl;
            motor_group->set_direction(d);
            delay(1000);
            motor_group->stop();
            delay(200);
        }
    }
    delay(100000);
}

arm::arm(int pin_id) : pin(pin_id) { current_angle = 0; }

void arm::rotate_to(int angle) {
    current_angle = angle;
    // todo communicate with hardware
}

void arm::rotate_by(int rel) { rotate_to(current_angle + rel); }

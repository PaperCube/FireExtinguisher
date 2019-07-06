#include "controllers.h"
#include "math_util.h"
#include "sout.h"
#include <Arduino.h>

using serial::endl;
using serial::sdebug;

motor_controller::motor_controller() {}

void motor_controller::init(int _pwm_pin, int _direction_pin) {
    pinMode(_direction_pin, OUTPUT);
    pwm_pin       = _pwm_pin;
    direction_pin = _direction_pin;
}

void motor_controller::stop() {
    digitalWrite(direction_pin, LOW);
    analogWrite(pwm_pin, 0);
}

void motor_controller::go(int speed) {
    int new_speed     = (int)(max_speed / 100.0 * speed);
    int new_speed_abs = abs(new_speed);
    if (new_speed_abs > 255) {
        new_speed_abs = 255;
        new_speed     = math::copy_sign(new_speed, 255);
    }
    bool backwards = new_speed < 0;
    if (new_speed == 0) {
        stop();
        return;
    }
    // sdebug << "motor_controller::go(int) value passed in :" << speed
    //        << " new_speed = " << new_speed << endl;
    digitalWrite(direction_pin, backwards ? HIGH : LOW);
    analogWrite(pwm_pin, backwards ? (255 - new_speed_abs) : new_speed_abs);
}

void motor_controller::go() { go(100); }

motor_pair::motor_pair(motor_controller *motor_l, motor_controller *motor_r) {
    left = motor_l, right = motor_r;
}

void motor_pair::stop() {
    left->stop();
    right->stop();
}

void motor_pair::go() {
    left->go();
    right->go();
}

void motor_pair::go(int v) {
    current_speed = v;
    left->go(v);
    right->go(v);
}

/**
 * Fully left: -1.0
 * Straight: 0.0
 * Fully right: 1.0
 */
// void motor_pair::turn(float v){
//    // not implemented
// }

quad_directional::quad_directional(motor_pair *lr_motor_pair,
                                   direction   def_lr,
                                   motor_pair *vertical_motor_pair,
                                   direction   def_vert) {
    pair_lr = lr_motor_pair;
    pair_fb = vertical_motor_pair;
    if (def_lr == direction::FORWARD) {
        multiplier_lr = 1;
    } else {
        multiplier_lr = -1;
    }
    if (def_vert == direction::RIGHTWARD) {
        multiplier_fb = 1;
    } else {
        multiplier_fb = -1;
    }
}

void quad_directional::stop() {
    pair_lr->stop();
    pair_fb->stop();
}

void quad_directional::set_direction(direction d) { 
    stop();
    current_direction = d; 
    go(current_speed);
}

void quad_directional::go() { go(100); }

void quad_directional::go(int v) {
    current_speed = v;
    int lr_manip = 0, fb_manip = 0;
    switch (current_direction) {
    case FORWARD:
        lr_manip = 1;
        break;
    case BACKWARD:
        lr_manip = -1;
        break;
    case RIGHTWARD:
        fb_manip = 1;
        break;
    case LEFTWARD:
        fb_manip = -1;
        break;
    }
    auto speed_lr = v * multiplier_lr * lr_manip;
    auto speed_fb = v * multiplier_fb * fb_manip;
    pair_lr->go(speed_lr);
    pair_fb->go(speed_fb);
}

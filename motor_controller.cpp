#include "motor_controller.h"
#include <Arduino.h>
#include "math_util.h"
#include "sout.h"

using serial::endl;
using serial::sdebug;

motor_controller::motor_controller() {}

void motor_controller::init(int _pwm_pin, int _direction_pin) {
    pinMode(_direction_pin, OUTPUT);
    pwm_pin = _pwm_pin;
    direction_pin = _direction_pin;
}

void motor_controller::stop() {
    digitalWrite(direction_pin, LOW);
    analogWrite(pwm_pin, 0);
}

void motor_controller::go(int speed) {
    int new_speed = (int)(max_speed / 100.0 * speed);
    int new_speed_abs = abs(new_speed);
    if (new_speed_abs > 255) {
        new_speed_abs = 255;
        new_speed = math::copy_sign(new_speed, 255);
    }
    bool backwards = new_speed < 0;
    if (new_speed == 0) {
        stop();
        return;
    }
    sdebug << "motor_controller::go(int) value passed in :" << speed
           << " new_speed = " << new_speed << endl;
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
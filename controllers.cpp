#include "controllers.h"
#include "math_util.h"
#include "sout.h"
#include <Arduino.h>

using serial::endl;
using serial::sdebug;

builtin_motor_driver::builtin_motor_driver(int port) {
    if (port <= 0 || port > 6)
        port = 1;
    digital_l = 38 + 2 * (6 - port);
    digital_r = digital_l + 1;
    pinMode(digital_l, OUTPUT);
    pinMode(digital_r, OUTPUT);
    analog_pin   = 8 + (6 - port);
    all_low_stop = true;
}

void builtin_motor_driver::go(int speed) {
    if (speed == 0) {
        digitalWrite(digital_l, all_low_stop ? LOW : HIGH);
        digitalWrite(digital_r, all_low_stop ? HIGH : LOW);
        analogWrite(analog_pin, 0);
        return;
    }
    digitalWrite(digital_l, speed > 0 ? HIGH : LOW);
    digitalWrite(digital_r, speed > 0 ? LOW : HIGH);
    analogWrite(analog_pin, math::absolute(speed));
}

direction reverse_direction(direction_t d) {
    static const direction rev_directions[] = {
        direction::BACKWARD, direction::RIGHTWARD, direction::LEFTWARD,
        direction::FORWARD};
    return rev_directions[(int)d];
}

motor_controller::motor_controller() {}

motor_controller::motor_controller(int _pwm_pin, int _direction_pin)
    : motor_controller() {
    init(_pwm_pin, _direction_pin);
}

void motor_controller::init(int _pwm_pin, int _direction_pin) {
    pinMode(_direction_pin, OUTPUT);
    pwm_pin       = _pwm_pin;
    direction_pin = _direction_pin;
}

void motor_controller::stop() {
    digitalWrite(direction_pin, LOW);
    analogWrite(pwm_pin, 0);
}

void motor_controller::power(int p) {
    double power_f     = p;
    double power_abs_f = math::absolute(power_f);
    if (power_abs_f > 255) {
        power_abs_f = 255;
        power_f     = math::copy_sign(power_f, 255.0);
    }
    // conversion should be done on float point types to prevent negative values
    // caused by overflow

    int  power_abs = (int)power_abs_f;
    int  power     = (int)power_f;
    bool backwards = power < 0;
    if (power == 0) {
        stop();
        sdebug << "Stopping " << direction_pin << " " << pwm_pin << endl;
        return;
    }
    sdebug << "motor_controller::power(int) value passed in :" << p << endl;
    sdebug << "direction: " << direction_pin << " pwm: " << pwm_pin << endl;
    digitalWrite(direction_pin, backwards ? HIGH : LOW);
    analogWrite(pwm_pin, backwards ? (255 - power_abs) : power_abs);
}

void motor_controller::go(int speed) { power(max_power / 100.0 * speed); }

void motor_controller::go() { go(100); }

motor_pair::motor_pair(motor_controller *motor_l, motor_controller *motor_r) {
    left = motor_l, right = motor_r;
}

void motor_pair::stop() {
    current_speed = 0;
    left->stop();
    right->stop();
}

void motor_pair::go() { go(100); }

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
    this->current_speed = 0;
}

void quad_directional::set_direction(direction_t d) {
    int original_speed = current_speed;
    stop();
    current_direction = (direction)d;
    go(original_speed);
}

void quad_directional::go() { go(100); }

void quad_directional::go(int v) {
    sdebug << "quad_directional - going " << v << endl;
    current_speed = v;
    int lr_manip = 0, fb_manip = 0;
    switch (current_direction) {
    case direction::FORWARD:
        lr_manip = 1;
        break;
    case direction::BACKWARD:
        lr_manip = -1;
        break;
    case direction::RIGHTWARD:
        fb_manip = 1;
        break;
    case direction::LEFTWARD:
        fb_manip = -1;
        break;
    }
    auto speed_lr = v * multiplier_lr * lr_manip;
    auto speed_fb = v * multiplier_fb * fb_manip;
    pair_lr->go(speed_lr);
    pair_fb->go(speed_fb);
}

void quad_directional::reverse_and_stop(int timeout_millis) {
    reverse_and_stop(-100, timeout_millis);
}

void quad_directional::reverse_and_stop(int power, int timeout_millis) {
    go(-math::absolute(power));
    delay(timeout_millis);
    stop();
}

arm::arm(int pin_id) : pin(pin_id) { current_angle = 0; }

void arm::rotate_to(int angle) {
    current_angle = angle;
    // todo communicate with hardware
}

void arm::rotate_by(int rel) { rotate_to(current_angle + rel); }
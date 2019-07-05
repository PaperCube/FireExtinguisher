#include "detector.h"
#include "display.h"
#include "motor_controller.h"
#include "sout.h"
#include <Arduino.h>

using namespace serial;

const int PORT_PROX_SENSOR = A6;
const int MOTOR_L = 3;
const int MOTOR_R = 4;
const int FULL_SPEED = 120;
const int NATURAL_CONSTANT = 2.71828;
const int STOP_THRESHOLD = -1;

int convert(int value) {
    return 148.6 * pow(NATURAL_CONSTANT,
                       -0.005 * value);
}

motor_controller motor_l;
motor_controller motor_r;
motor_pair *mot_pair = nullptr;

int read_sensor_raw() { return analogRead(PORT_PROX_SENSOR); }

int read_sensor_raw_calibrated() {
    const int REPEAT_CNT = 200;
    double sum = 0;
    for (int i = 0; i < REPEAT_CNT; i++) {
        sum += read_sensor_raw();
    }
    return (int)(sum / REPEAT_CNT);
}

int read_sensor() { return convert(read_sensor_raw_calibrated()); }

void detector::setup() {
    // motor_controller *motor = new motor_controller;
    // motor->init(3, 27);
    // motor->max_speed = 100;
    // motor->go();
    motor_l.init(3, 27);
    motor_l.max_speed = 70;
    motor_r.init(2, 26);
    motor_r.max_speed = 70;
    mot_pair = new motor_pair(&motor_l, &motor_r);
    display::reset();
    display::update_display();
    sdebug << "Updating display" << endl;
}

void detector::run() {
    while (true) {
        int sensor_value = read_sensor_raw_calibrated();
        int val_converted = convert(sensor_value);
        display::sensor_value = sensor_value;
        display::sensor_value_converted = val_converted;
        display::update_display();
        sdebug << "Sensor : " << sensor_value << " | converting to " 
               << convert(sensor_value) << endl;

        if (val_converted < STOP_THRESHOLD) {
            mot_pair->stop();
        } else {
            // todo check whether this statement is essential.
            // mot_pair->go();
        }
    }
}
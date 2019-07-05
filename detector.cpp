#include "detector.h"
#include "display.h"
#include "motor_controller.h"
#include "sout.h"
#include <Arduino.h>

using namespace serial;

#define PORT_PROX_SENSOR A7
#define MOTOR_L 3
#define MOTOR_R 4
#define FULL_SPEED 120
#define NATURAL_CONSTANT 2.71828
#define STOP_THRESHOLD 20

int convert(int value) {
    return 148.6 * pow(NATURAL_CONSTANT,
                       -0.005 * value);
}

motor_controller motor_l(MOTOR_L);
motor_controller motor_r(MOTOR_R);

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
    motor_l.go();
    motor_r.go();
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
            motor_l.stop();
            motor_r.stop();
        } else {
            motor_l.go();
            motor_r.go();
        }
    }
}
#include <Arduino.h>

#include "sensors.h"

const int NATURAL_CONSTANT = 2.71828;

const int    prox_sensor::builtin_prox_sensor_pins[4] = {A1, A3, A4, A2};
prox_sensor *prox_sensor::prox_sensors[4]             = {nullptr};

int prox_sensor::convert(int value) {
    return 148.6 * pow(NATURAL_CONSTANT, -0.005 * value);
}

int prox_sensor::read_raw() { return analogRead(pin); }

int prox_sensor::read() { return convert(read_raw_calibrated()); }

int prox_sensor::read_raw_calibrated() {
    const int REPEAT_CNT = 200;
    long long sum        = 0;
    for (int i = 0; i < REPEAT_CNT; i++) {
        sum += read_raw();
    }
    return (int)((double)sum / REPEAT_CNT);
}

prox_sensor::prox_sensor(int pin_id) { pin = pin_id; }

bool are_builtin_sensors_setup = false;

void prox_sensor::prepare_builtin_sensors() {
    if (are_builtin_sensors_setup)
        return;
    are_builtin_sensors_setup = true;
    for (int i = 0; i < 4; i++) {
        prox_sensors[i] = new prox_sensor(builtin_prox_sensor_pins[i]);
    }
}

prox_sensor *prox_sensor::sensor_at(direction d) {
    prepare_builtin_sensors();
    return prox_sensors[(int)d];
}

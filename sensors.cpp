#include <Arduino.h>

#include "sensors.h"
#include "sout.h"

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

namespace sensor_debugging {

int read(int pin) {
    const int rep = 100;
    long      sum = 0;
    for (int i = 0; i < rep; i++) {
        sum += analogRead(pin);
    }
    return sum / rep;
}

void fill(char *buf, int value, int len) {
    static char locbuf[64];
    int         pos = 0;
    if (value == 0) {
        locbuf[0] = '0';
        pos++;
    }
    while (value != 0) {
        locbuf[pos++] = '0' + value % 10;
        value /= 10;
    }
    const int num_len     = pos;
    const int filler_size = len - num_len;
    for (int i = 0; i < filler_size; i++) {
        buf[i] = ' ';
    }
    for (int i = 0; i < num_len; i++) {
        buf[filler_size + i] = locbuf[num_len - 1 - i];
    }
    buf[len] = 0;
}

void debug_show_values() {
    using namespace serial;
    while (true) {
        char buf[64];
        for (int i = 1; i <= 8; i++) {
            sensor_debugging::fill(buf, sensor_debugging::read(i), 5);
            sdebug << String(buf) << "|";
        }
        sdebug << endl;
        delay(200);
    }
}

} // namespace sensor_debugging
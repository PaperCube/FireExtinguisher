#include <Arduino.h>

#include "parameters.h"
#include "sensors.h"
#include "sout.h"

const int NATURAL_CONSTANT = 2.71828;

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

sensor_pair::sensor_pair(prox_sensor *sl, prox_sensor *sr) {
    l = sl;
    r = sr;
}

int sensor_pair::read() {
    int vl = read_l();
    int vr = read_r();
    return (vl + vr) / 2;
}

int sensor_pair::read_raw() {
    int vl = get_left()->read_raw_calibrated();
    int vr = get_right()->read_raw_calibrated();
    return (vl + vr) / 2;
}

int sensor_pair::read_l() { return l->read(); }

int sensor_pair::read_r() { return r->read(); }

prox_sensor *sensor_pair::get_left() { return l; }

prox_sensor *sensor_pair::get_right() { return r; }

sensor_manager sensor_manager::instance;

sensor_manager::sensor_manager() { is_prepared = false; }

sensor_manager::sensor_type *sensor_manager::sensor_at(direction d) {
    prepare();
    return this->sensors[(int)d];
}

void sensor_manager::prepare() {
    if (is_prepared)
        return;
    is_prepared                 = true;
    const int sensor_ordinals[] = {1, 8, 3, 2, 7, 6, 5, 4};
    for (int i = 0; i < 4; i++) {
        prox_sensor *l = new prox_sensor(
                        SENSOR_PINS[sensor_ordinals[i * 2] - 1]),
                    *r = new prox_sensor(
                        SENSOR_PINS[sensor_ordinals[i * 2 + 1] - 1]);
        sensors[i] = new sensor_pair(l, r);
    }
}
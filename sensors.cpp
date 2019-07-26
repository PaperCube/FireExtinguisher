#include <Arduino.h>

#include "buttons.h"
#include "buzzer.h"
#include "math_util.h"
#include "parameters.h"
#include "sensors.h"
#include "sout.h"

using namespace serial;

const int NATURAL_CONSTANT = 2.71828;

BiIntFunction sensor_coordinations[2];

int by_average(int a, int b) { return a + (b - a) / 2; }
int by_min(int a, int b) { return math::min_of(a, b); }

int prox_sensor::convert(int value) {
    if (value == 0)
        value = 1;
    return 30000 / value;
}

int prox_sensor::read_raw() {
    int value = analogRead(pin);
    if (value < 80)
        value = 80;
    else if (value > 700)
        value = 700;
    return value;
}

int prox_sensor::read() { return convert(read_raw_calibrated()); }

int prox_sensor::read_raw_calibrated() {
    for (int i = 0; i < FILTER_BUFFER_SIZE; i++) {
        buf[i] = read_raw();
    }
    alg::merge_sort(buf, buf + FILTER_BUFFER_SIZE, sort_buf);
    const int l = 8, r = 24;
    long      sum = 0;
    for (int i = l; i < r; i++) {
        sum += buf[i];
    }
    return sum / (r - l);
}

int prox_sensor::get_pin() { return pin; }

prox_sensor::prox_sensor(int pin_id) { pin = pin_id; }

sensor_pair::sensor_pair(prox_sensor *sl, prox_sensor *sr) {
    l = sl;
    r = sr;
}

int sensor_pair::read() {
    long vl = read_l();
    long vr = read_r();
    return sensor_coordinations[SENSOR_COORDINATION_TYPE](vl, vr);
}

int sensor_pair::read_raw() {
    long vl = get_left()->read_raw_calibrated();
    long vr = get_right()->read_raw_calibrated();
    return sensor_coordinations[SENSOR_COORDINATION_TYPE](vl, vr);
}

int sensor_pair::read_l() { return l->read(); }

int sensor_pair::read_r() { return r->read(); }

prox_sensor *sensor_pair::get_left() { return l; }

prox_sensor *sensor_pair::get_right() { return r; }

sensor_manager sensor_manager::instance;

sensor_manager::sensor_manager() { is_prepared = false; }

sensor_manager::sensor_type *sensor_manager::sensor_at(direction_t d) {
    prepare();
    return this->sensors[(int)d];
}

void sensor_manager::prepare() {
    if (is_prepared)
        return;
    is_prepared                 = true;
    const int sensor_ordinals[] = {1, 8, 3, 2, 5, 4, 7, 6};
    for (int i = 0; i < 4; i++) {
        prox_sensor *l = new prox_sensor(
                        SENSOR_PINS[sensor_ordinals[i * 2] - 1]),
                    *r = new prox_sensor(
                        SENSOR_PINS[sensor_ordinals[i * 2 + 1] - 1]);
        sensors[i] = new sensor_pair(l, r);
    }

    sensor_coordinations[1] = by_average;
    sensor_coordinations[2] = by_min;
}

void sensor_manager::set_coordination_function(BiIntFunction f) {
    sensor_coordinations[0]  = f;
    SENSOR_COORDINATION_TYPE = 0;
}

/*
 * =================================
 * begin: namespace sensor_debugging
 */

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

int debug_show_values() {
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

void __interactive_test(int *pins) {
    sout.init();
    button b(1);
    long   first_press = -1;
    struct info {
        bool        pass = false;
        prox_sensor sens;
        int         min, max;

        info() : sens(-1) { pass = false; }
        ~info() {}
    } infos[8];
    _loop(i, 8) {
        infos[i].sens = prox_sensor(pins[i]);
        infos[i].min  = 0x7FFF;
        infos[i].max  = 0x8000;
    }
    int  pass_cnt    = 0;
    bool interrupted = false;
    sout << "Beginning interactive sensor test. Hold button 1 to quit" << endl;
    while (true) {
        if (pass_cnt >= 8)
            break;
        if (b.is_pressed()) {
            sdebug << "B is pressed" << endl;
            if (first_press < 0)
                first_press = millis();
            else if (millis() - first_press > 1500) {
                interrupted = true;
                break;
            }
        }
        for (int i = 0; i < 8; i++) {
            info &cur_info = infos[i];
            if (cur_info.pass) {
                continue;
            }
            const int num = i + 1;
            int       v   = cur_info.sens.read();
            cur_info.min  = math::min_of(v, cur_info.min);
            cur_info.max  = math::max_of(v, cur_info.max);
            if (cur_info.max - cur_info.min > 180) {
                cur_info.pass = true;
                pass_cnt++;
                sout << "Sensor #" << num << " (pin " << pins[i]
                     << ") is detected" << endl;
            }
        }
    }
    if (interrupted) {
        sout << "Interactive test cancelled by user" << endl;
    } else {
        sout << "Test done" << endl;
    }
}

} // namespace sensor_debugging
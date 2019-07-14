#include <Arduino.h>

#include "light.h"

const int PIN_R = 36;
const int PIN_G = 37;
const int PIN_B = 35;

light *light::built_in_light = nullptr;

light *light::get_light() {
    int pins[] = {PIN_R, PIN_G, PIN_B};
    if (built_in_light == nullptr) {
        built_in_light = new light(pins);
    }
    return built_in_light;
}

light::light(int _pins[3]) {
    for (int i = 0; i < 3; i++) {
        int cur_pin;
        cur_pin = pins[i] = _pins[i];
        pinMode(cur_pin, OUTPUT);
        digitalWrite(cur_pin, LOW);
    }
}

void light::turn_off() { set_color({0, 0, 0}); }

void light::set_color(unsigned r, unsigned g, unsigned b) {
    unsigned values[] = {r, g, b};
    for (int i = 0; i < 3; i++) {
        digitalWrite(pins[i], values[i] == 0 ? LOW : HIGH);
    }
}

void light::set_color(rgb c) { set_color(c.r, c.g, c.b); }
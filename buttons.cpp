#include "buttons.h"
#include "sout.h"
#include <Arduino.h>

using namespace serial;

button::button(int button_pin) {
    pin_id    = 21 + button_pin;
    button_id = button_pin;
}

bool button::is_pressed() {
    const int REPEAT_COUNT = 50;
    int       ctr          = 0;
    for (int i = 0; i < REPEAT_COUNT; i++) {
        if (digitalRead(pin_id)) {
            ctr++;
        }
    }
    return ctr > (REPEAT_COUNT / 2);
}
void button::wait_until_released() {
    while (!is_pressed())
        ;
    while (is_pressed())
        ;
}
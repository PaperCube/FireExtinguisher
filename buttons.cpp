#include "buttons.h"
#include <Arduino.h>

bool button::is_pressed() { return digitalRead(pin_id) == 1; }
void button::wait_until_released() {
    while (!is_pressed())
        ;
    while (is_pressed())
        ;
}
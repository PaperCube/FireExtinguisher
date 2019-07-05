#include "display.h"
#include <Arduino.h>
#include <UTFT.h>

namespace display {

typedef const unsigned char FontByte;
UTFT LCD(QD_TFT180A, 51, 52, 32, 34, 33);
int sensor_value = 0;
int sensor_value_converted = 0;

const color white = {255, 255, 255};
const color black = {0, 0, 0};

int last_update_time = 0;
int update_interval = 200;

bool try_update_time() {
    int current = millis();
    if (current - last_update_time > update_interval) {
        last_update_time = current;
        return true;
    }
    return false;
}

void set_color(color c) { LCD.setColor(c.r, c.g, c.b); }

void update_display() {
    if (!try_update_time())
        return;
    set_color(black);
    LCD.fillRect(105, 0, 141, 24);
    set_color(white);
    LCD.print(String(sensor_value), 105, 0);
    LCD.print(String(sensor_value_converted), 105, 12);
}

void init_sensor_monitor() {
    LCD.print(String("sensor_val = "), 0, 0);
    LCD.print(String("calibrated = "), 0, 12);
}

void reset() {
    LCD.InitLCD(LANDSCAPE);
    LCD.clrScr();
    LCD.setFont(SmallFont);
    LCD.setColor(255, 255, 255);
    last_update_time = millis();
    init_sensor_monitor();
}

} // namespace display
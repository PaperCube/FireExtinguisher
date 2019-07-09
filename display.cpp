#include "display.h"
#include <Arduino.h>
#include <UTFT.h>

namespace display {

typedef const unsigned char FontByte;
UTFT                        LCD(QD_TFT180A, 51, 52, 32, 34, 33);

param_container params;

const color white = {255, 255, 255};
const color black = {0, 0, 0};

int last_update_time = 0;
int update_interval  = 500;

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
    LCD.fillRect(85, 0, 120, 12);
    set_color(white);
    LCD.print(String(params.loop_avg), 85, 0);
}

void init_sensor_monitor() {
    LCD.print(String("loop_avg = "), 0, 0);
    LCD.print(String("0123456789abcdefghijklmnopqrst"), 0, 60);
}

void reset() {
    LCD.InitLCD(LANDSCAPE);
    LCD.clrScr();
    LCD.setFont(const_cast<unsigned char *>(SmallFont));
    LCD.setColor(255, 255, 255);
    last_update_time = millis();
    init_sensor_monitor();
}

} // namespace display
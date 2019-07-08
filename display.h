#ifndef DISPLAY_H
#define DISPLAY_H

#include <UTFT.h>

namespace display {

#include "DefaultFonts.c"

extern UTFT LCD;
extern int sensor_value;
extern int sensor_value_converted;

struct color {
    unsigned char r, g, b;
};

extern const color white;
extern const color black;

void set_color(color c);

void reset();

void init_sensor_monitor();

void update_display();

} // namespace display

#endif
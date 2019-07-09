#ifndef DISPLAY_H
#define DISPLAY_H

#include <UTFT.h>

namespace display {

#include "DefaultFonts.c"

struct param_container{
    int loop_avg;
};

extern param_container params;

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
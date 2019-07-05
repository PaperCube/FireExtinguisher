#ifndef DETECTOR_H
#define DETECTOR_H

#include <Arduino.h>

int convert(int);

struct detector{
    void setup();
    void run();
};

#endif
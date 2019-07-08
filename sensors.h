#ifndef SENSORS_H
#define SENSORS_H

#include "definitions.h"

class prox_sensor {
  private:
    int                 pin;
    int                 convert(int);
    static const int    builtin_prox_sensor_pins[4];
    static prox_sensor *prox_sensors[4];

  public:
    prox_sensor(int);
    int read_raw();
    int read_raw_calibrated();
    int read();

    static prox_sensor *sensor_at(direction);

    static void prepare_builtin_sensors();
};
#endif
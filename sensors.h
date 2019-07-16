#ifndef SENSORS_H
#define SENSORS_H

#include "definitions.h"

class prox_sensor {
  private:
    int pin;
    int convert(int);

  public:
    prox_sensor(int);
    int read_raw();
    int read_raw_calibrated();
    int read();

    static void debug_show_values();
};

class sensor_pair {
  private:
    prox_sensor *l, *r;

  public:
    sensor_pair(prox_sensor *sl, prox_sensor *sr);
    int          read();
    int          read_l();
    int          read_r();
    prox_sensor *get_left();
    prox_sensor *get_right();
};

class sensor_manager {
  public:
    typedef sensor_pair   sensor_type;
    static sensor_manager instance;

  private:
    bool is_prepared;

  public:
    sensor_type *sensors[4];
    sensor_manager();
    sensor_type *sensor_at(direction);
    void prepare();
};
#endif
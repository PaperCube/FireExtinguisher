#ifndef SENSORS_H
#define SENSORS_H

#include "definitions.h"
#include "parameters.h"

typedef int (*BiIntFunction)(int, int);

extern BiIntFunction sensor_coordinations[2];

class prox_sensor {
  private:
    static const int FILTER_BUFFER_SIZE = 32;

    int pin;
    int convert(int);
    int buf[FILTER_BUFFER_SIZE];
    int sort_buf[FILTER_BUFFER_SIZE];

  public:
    prox_sensor(int);
    int read_raw();
    int read_raw_calibrated();
    int read();
    int get_pin();
};

class sensor_pair {
  private:
    prox_sensor *l, *r;

  public:
    sensor_pair(prox_sensor *sl, prox_sensor *sr);
    int          read();
    int          read_raw();
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
    sensor_type *sensor_at(direction_t);
    void         prepare();
    void         set_coordination_function(BiIntFunction);
};

namespace sensor_debugging {

int  debug_show_values();
void __interactive_test(int *);

} // namespace sensor_debugging
#endif
#ifndef LIGHT_H
#define LIGHT_H

struct rgb {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

class light {
    int           pins[3];
    static light *built_in_light;

  public:
    static light *get_light();

    light(int[3]);
    void turn_off();
    void set_color(rgb c);
    void set_color(unsigned, unsigned, unsigned);
};

#endif 
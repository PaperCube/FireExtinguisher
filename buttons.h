#ifndef BUTTONS_H
#define BUTTONS_H

class button {
  private:
    int pin_id;
    int button_id;

  public:
    button(int);
    // void set_hold_timeout(long v);
    bool is_pressed();
    void wait_until_released();
};

#endif
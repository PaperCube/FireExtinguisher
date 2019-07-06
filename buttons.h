#ifndef BUTTONS_H
#define BUTTONS_H

class button {
   private:
    int pin_id;
    int button_id;
   public:
    button(int button) {
        pin_id = 21 + button;
        button_id = button;
    }

    bool is_pressed();
    void wait_until_released();
};

#endif
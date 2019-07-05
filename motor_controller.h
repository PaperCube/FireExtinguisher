#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

struct motor_controller {
   public:
    int pwm_pin;
    int direction_pin;
    int max_speed = 150;

    motor_controller();
    void init(int, int);
    void stop();
    void go(int speed);
    void go();

   private:
};

struct motor_pair {
   public:
    motor_pair(motor_controller*, motor_controller*);
    void stop();
    void go(int);
    void go();
    // void turn(float);
   private:
    motor_controller *left, *right;
    int current_speed;
   protected:
};

#endif MOTOR_CONTROLLER_H
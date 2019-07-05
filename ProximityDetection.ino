#include "detector.h"
#include "sout.h"

using namespace serial;

detector det;

void set_all(int v) {
    for (int i = 2; i <= 5; i++) {
        analogWrite(i, i % 2 == 0 ? v : 0);
    }
}

void setup() { 
    const int pwm = 50;

    for (int i = 2; i <= 5; i++) {
        pinMode(i, OUTPUT);
    }

    while (true) {
        for (int i = 50; i <= 150; i++) {
            set_all(i > 100 ? 200 - i : i);
            delay(60);
        }
    }

    return;
    sout.init();
    sdebug << "Initializing..." << endl;
    det.setup();
    det.run();
}

void loop() {
    // put your main code here, to run repeatedly:
}

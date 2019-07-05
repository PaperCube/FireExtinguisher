#include "detector.h"
#include "motor_controller.h"
#include "sout.h"

using namespace serial;

detector det;

void set_all(int v) {
    for (int i = 2; i <= 5; i++) {
        analogWrite(i, i % 2 == 0 ? v : 0);
    }
}

void setup() {
    sout.init();
    sdebug << "Initializing..." << endl;
    det.setup();
    det.run();
}

void loop() {
    // put your main code here, to run repeatedly:
}

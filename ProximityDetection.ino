#include "robot.h"
#include "controllers.h"
#include "sout.h"

using namespace serial;

robot r;

void setup() {
    sout.init();
    sdebug << "Initializing..." << endl;
    r.setup();
    r.run();
}

void loop() {
    // put your main code here, to run repeatedly:
}

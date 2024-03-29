#include "sout.h"
#include <Arduino.h>

namespace serial {

void SerialOut::init(unsigned int rate) { Serial.begin(rate); }

SerialOut sout;
SerialOut sdebug(true);

} // namespace serial

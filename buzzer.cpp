#include <Arduino.h>

#include "buzzer.h"
#include "sout.h"

using namespace serial;

buzz_pattern::buzz_pattern() {}

namespace buzz_patterns {
bool is_prepared = false;
buzz_pattern DOUBLE_SHORT;
buzz_pattern SHORT;
buzz_pattern LONG;
buzz_pattern DOUBLE_LONG;

void prepare() {
    if(is_prepared) return;
    is_prepared = true;
    DOUBLE_SHORT.set({100, 100, 100});
    DOUBLE_LONG.set({1000, 500, 1000});
    LONG.set({1000});
    SHORT.set({100});
}
} // namespace buzz_patterns

const int BUZZER_PIN = 31;

buzzer::buzzer() { pinMode(BUZZER_PIN, OUTPUT); }

void buzzer::buzz() { digitalWrite(BUZZER_PIN, HIGH); }

void buzzer::stop() { digitalWrite(BUZZER_PIN, LOW); }

void buzzer::buzz(int timeout) {
    buzz();
    delay(timeout);
    stop();
}

void buzzer::buzz(buzz_pattern &pattern) {
    for (int i = 0; i < pattern.length; i++) {
        if ((i & 1) == 0) {
            buzz(pattern.pattern[i]);
        } else {
            stop();
            delay(pattern.pattern[i]);
        }
    }
}
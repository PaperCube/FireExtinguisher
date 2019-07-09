#ifndef BUZZER_H
#define BUZZER_H

class buzz_pattern {
  public:
    buzz_pattern();
    int    length;
    short *pattern;

    template <int L> void set(int(&&pat)[L]) {
        length  = L;
        pattern = new short[L];
        for (int i = 0; i < L; i++) {
            pattern[i] = (short)pat[i];
        }
    }
};

namespace buzz_patterns {
extern buzz_pattern DOUBLE_SHORT;
extern buzz_pattern SHORT;
extern buzz_pattern LONG;
extern buzz_pattern DOUBLE_LONG;
extern void         prepare();
} // namespace buzz_patterns

class buzzer {
  public:
    buzzer();
    void buzz();
    void stop();
    void buzz(int);
    void buzz(buzz_pattern &);
};

#endif
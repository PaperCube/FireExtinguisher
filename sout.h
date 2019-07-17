#ifndef SOUT_H
#define SOUT_H

#include <WString.h>
#include <Arduino.h>

namespace serial
{
class SerialOut
{
  private:
    bool enabled;

  public:
    SerialOut(bool flag = true) : enabled(flag) {}
    ~SerialOut() {}
    SerialOut(const SerialOut &) = delete;
    SerialOut(const SerialOut &&) = delete;
    SerialOut& operator=(const SerialOut &) = delete;
    SerialOut& operator=(const SerialOut &&) = delete;

    template <typename T>
    SerialOut &operator<<(const T &any)
    {
        if (!enabled)
            return *this;
        // init();
        Serial.print(any);
        return *this;
    }

    template <typename T>
    SerialOut &operator<<(T &&any)
    {
        if (!enabled)
            return *this;
        // init();
        Serial.print(any);
        return *this;
    }

    operator bool()
    {
        return enabled;
    }

    void init(unsigned int rate = 9600);
};

const String endl = "\n";
extern SerialOut sout;
extern SerialOut sdebug;
}

#endif
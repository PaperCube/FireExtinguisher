#ifndef SOUT_H
#define SOUT_H

#include <WString.h>
#include <Arduino.h>

namespace serial
{
static bool hasBegun = false;
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

static const String endl = "\n";
static SerialOut sout;
static SerialOut sdebug(true);
}

#endif
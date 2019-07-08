#ifndef MATH_UTIL_H
#define MATH_UTIL_H

namespace math {

template <class T> T absolute(T value) {
    if (value < 0)
        return -value;
    return value;
}

template <class T> T copy_sign(T sign, T value) {
    return sign < 0 ? -absolute(value) : absolute(value);
}

} // namespace math

#endif
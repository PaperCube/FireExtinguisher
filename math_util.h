#ifndef MATH_UTIL_H
#define MATH_UTIL_H

#define _loop(v, a) for (int(v) = 0; (v) < (a); (v)++)

namespace math {

template <class T> T absolute(T value) {
    if (value < 0)
        return -value;
    return value;
}

template <class T> T copy_sign(T sign, T value) {
    return sign < 0 ? -absolute(value) : absolute(value);
}

template <class T, class R> void copy_arr(T *from_begin, T *from_end, R *to) {
    int sz = from_end - from_begin;
    for (int i = 0; i < sz; i++) {
        to[i] = from_begin[i];
    }
}

template <class T> void fill(T *begin, T *end, const T v) {
    for (T *iter = begin; iter != end; iter++) {
        *iter = v;
    }
}

template <class T> T max_of(T a, T b) { return a > b ? a : b; }

extern template int max_of(int a, int b);

template <class T> T min_of(T a, T b) { return a < b ? a : b; }

extern template int min_of(int a, int b);

double round_d(double value);

class averager {
    int  cnt;
    long sum;

  public:
    averager();
    void   update(int v);
    double get_avg();
};

} // namespace math

#endif
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

template <class T, class R> void copy_arr(T *from_begin, T *from_end, R *to){
    int sz = from_end - from_begin;
    for(int i = 0; i < sz; i++){
        to[i] = from_begin[i];
    }
}

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
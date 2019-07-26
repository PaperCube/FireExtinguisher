#ifndef MATH_UTIL_H
#define MATH_UTIL_H

#define _loop(v, a) for (int(v) = 0; (v) < (a); (v)++)

template <class T, class SumType, int L> class cyclic_queue {
    T   values[L];
    int front;
    int rear;
    int len;

    constexpr static int max_size = L;

    SumType sum;

  private:
    void normalize() {
        if (front > L) {
            front %= L;
            rear = front + len;
        }
    }

    T &get_internal(int index) { return values[index % L]; }

  public:
    cyclic_queue() {
        front = 0;
        rear  = 0;
    }
    int  size() { return len; }
    void push_back(T v) {
        if (len >= L) {
            pop_front();
        }
        get_internal(rear++) = v;
        len++;
        sum += v;
        normalize();
    }
    T &  operator[](int v) { return get_internal(front + v); }
    void pop_front() {
        if (len > 0) {
            len--;
            sum -= get_internal(front++);
        }
    }
    void pop_back() {
        if (len > 0) {
            len--;
            sum -= get_internal(rear--);
        }
    }
    void clear() {
        front = rear = 0;
        len = sum = 0;
    }
    SumType get_sum() { return sum; }
};

extern template class cyclic_queue<int, long, 30>;

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
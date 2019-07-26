#ifndef MATH_UTIL_H
#define MATH_UTIL_H

#define _loop(v, a) for (int(v) = 0; (v) < (a); (v)++)

namespace alg {

template <typename T> void merge_sort(T *, T *, T *);

template <typename T> void merge(T *, T *, T *, T *);

template <typename T> T *extract_new(T *, T *, T *);

template <typename T> void merge_sort(T *begin, T *end, T *buf) {
    if (end - begin > 1) {
        T *center = begin + (end - begin) / 2 + 1;
        merge_sort(begin, center, buf);
        merge_sort(center, end, buf);
        merge(begin, center, end, buf);
    }
}

template <typename T> void merge(T *begin, T *center, T *end, T *buf) {
    int l_len = center - begin;
    // int r_len = end - center;

    T *l = extract_new(begin, center, buf);
    T *r = extract_new(center, end, buf + l_len);

    int pl, pr;
    pl = pr = 0;

    T *iter = begin;
    while (iter != end) {
        if (pl < l_len && l[pl] <= r[pr]) {
            *iter = l[pl++];
        } else {
            *iter = r[pr++];
        }
        iter++;
    }

    // delete[] l;
    // delete[] r;
}

template <typename T> T *extract_new(T *begin, T *end, T *buf) {
    int len       = end - begin;
    T * new_array = buf;
    for (int i = 0; i < len; i++) {
        new_array[i] = begin[i];
    }

    return new_array;
}

extern template void merge_sort(int *, int *, int *);
extern template void merge(int *, int *, int *, int *);
extern template int *extract_new(int *, int *, int *);

} // namespace alg

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
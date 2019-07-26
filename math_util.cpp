#include "math_util.h"

template class cyclic_queue<int, long, 30>;

namespace math {

averager::averager() {
    cnt = 0;
    sum = 0;
}

void averager::update(int v) {
    sum += v;
    cnt++;
}

double round_d(double value) {
    int    v    = (int)value;
    double diff = value - v;
    if (diff >= 0.5) {
        return v + 1;
    }
    return v;
}

double averager::get_avg() { return (double)sum / cnt; }

template int max_of(int a, int b);
template int min_of(int a, int b);

} // namespace math
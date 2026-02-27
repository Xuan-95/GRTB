#include "interval.h"

Interval createInterval(double min, double max) {
    Interval interval;
    interval.max = max;
    interval.min = min;
    return interval;
}
double size(Interval *interval) { return interval->max - interval->min; }
int contains(Interval *interval, double x) {
    return interval->min <= x && x <= interval->max;
}
int surrounds(Interval *interval, double x) {
    return interval->min < x && x < interval->max;
}

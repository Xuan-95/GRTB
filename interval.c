#include "interval.h"

Interval createInterval(double min, double max) {
    Interval interval;
    interval.max = max;
    interval.min = min;
    return interval;
}

Interval unionInterval(Interval a, Interval b) {
    Interval interval;
    interval.min = a.min <= b.min ? a.min : b.min;
    interval.max = a.max >= b.max ? a.max : b.max;
    return interval;
}

double size(Interval *interval) { return interval->max - interval->min; }

int    contains(Interval *interval, double x) { return interval->min <= x && x <= interval->max; }

int    surrounds(Interval *interval, double x) { return interval->min < x && x < interval->max; }

void   expand(Interval *interval, double delta) {
    double padding = delta / 2.0;
    interval->min  = interval->min - padding;
    interval->max  = interval->max + padding;
}

double clamp(Interval *interval, double x) {
    if (x < interval->min)
        return interval->min;
    if (x > interval->max)
        return interval->max;
    return x;
}

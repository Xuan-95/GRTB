#ifndef INTERVAL_H
#define INTERVAL_H

typedef struct {
    double min;
    double max;
} Interval;

Interval createInterval(double min, double max);
Interval unionInterval(Interval a, Interval b);
double   size(Interval *interval);
int      contains(Interval *interval, double x);
int      surrounds(Interval *interval, double x);
double   clampInterval(Interval *interval, double x);
void     expand(Interval *interval, double delta);

#endif // !INTERVAL_H

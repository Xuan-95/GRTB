#ifndef INTERVAL_H
#define INTERVAL_H

typedef struct {
    double min;
    double max;
} Interval;

Interval createInterval(double min, double max);
double size(Interval *interval);
int contains(Interval *interval, double x);
int surrounds(Interval *interval, double x);

#endif // !INTERVAL_H

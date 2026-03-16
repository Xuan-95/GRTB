#ifndef AABB_H
#define AABB_H

#include "common.h"
#include "interval.h"

typedef struct {
    Interval x;
    Interval y;
    Interval z;
} Aabb;

Aabb     createAabb(Interval x, Interval y, Interval z);
Aabb     createAabbFromPoints(Point3D a, Point3D b);
Aabb     unionAabb(Aabb a, Aabb b);
Interval axisInterval(Aabb *aabb, int n);
int      hitAabb(Aabb *aabb, Ray r, Interval ray_t);

#endif // !AABB_H

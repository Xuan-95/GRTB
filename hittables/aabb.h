#ifndef AABB_H
#define AABB_H

#include "../core/common.h"
#include "../math/interval.h"

typedef struct {
    Interval x;
    Interval y;
    Interval z;
} Aabb;

Aabb              createAabb(Interval x, Interval y, Interval z);
Aabb              createAabbFromPoints(Point3D a, Point3D b);
int               longestAxis(Aabb aabb);
Aabb              unionAabb(Aabb a, Aabb b);
Interval          axisInterval(Aabb *aabb, int n);
int               hitAabb(Aabb *aabb, Ray r, Interval ray_t);
Aabb              moveBbox(Aabb aabb, Vector3D offset);

extern const Aabb AABB_EMPTY;
extern const Aabb AABB_UNIVERSE;

#endif // !AABB_H

#include "aabb.h"
#include "interval.h"
#include <math.h>

Aabb padToMinimum(Aabb aabb) {
    double delta = 0.0001;
    if (size(&aabb.x) < delta)
        expand(&aabb.x, delta);
    if (size(&aabb.y) < delta)
        expand(&aabb.y, delta);
    if (size(&aabb.z) < delta)
        expand(&aabb.z, delta);
    return aabb;
}
Aabb createAabb(Interval x, Interval y, Interval z) {
    Aabb aabb;
    aabb.x = x;
    aabb.y = y;
    aabb.z = z;
    aabb   = padToMinimum(aabb);
    return aabb;
}

Aabb createAabbFromPoints(Point3D a, Point3D b) {
    Aabb aabb;
    aabb.x = (a.x <= b.x) ? createInterval(a.x, b.x) : createInterval(b.x, a.x);
    aabb.y = (a.y <= b.y) ? createInterval(a.y, b.y) : createInterval(b.y, a.y);
    aabb.z = (a.z <= b.z) ? createInterval(a.z, b.z) : createInterval(b.z, a.z);
    aabb   = padToMinimum(aabb);

    return aabb;
}

Aabb createEmptyAabb(void) {
    return createAabb(createInterval(+INFINITY, -INFINITY), createInterval(+INFINITY, -INFINITY),
                      createInterval(+INFINITY, -INFINITY));
}

Aabb createUniverseAabb(void) {
    return createAabb(createInterval(-INFINITY, +INFINITY), createInterval(-INFINITY, +INFINITY),
                      createInterval(-INFINITY, +INFINITY));
}

Aabb unionAabb(Aabb a, Aabb b) {
    Aabb aabb;
    aabb.x = unionInterval(a.x, b.x);
    aabb.y = unionInterval(a.y, b.y);
    aabb.z = unionInterval(a.z, b.z);
    return aabb;
}

Interval axisInterval(Aabb *aabb, int n) {
    switch (n) {
    case 0:
        return aabb->x;
    case 1:
        return aabb->y;
    case 2:
        return aabb->z;
    default:
        return aabb->x;
    }
}

int longestAxis(Aabb aabb) {
    double x_size = aabb.x.max - aabb.x.min;
    double y_size = aabb.y.max - aabb.y.min;
    double z_size = aabb.z.max - aabb.z.min;
    if (x_size > y_size) {
        return x_size > z_size ? 0 : 2;
    } else {
        return y_size > z_size ? 1 : 2;
    }
}

int hitAabb(Aabb *aabb, Ray r, Interval ray_t) {
    Point3D  origin    = r.origin;
    Vector3D direction = r.direction;

    for (int axis = 0; axis < 3; axis++) {
        Interval ax    = axisInterval(aabb, axis);
        double   adinv = 1.0 / direction.e[axis];

        double   t0 = (ax.min - origin.e[axis]) * adinv;
        double   t1 = (ax.max - origin.e[axis]) * adinv;

        if (t0 < t1) {
            if (t0 > ray_t.min)
                ray_t.min = t0;
            if (t1 < ray_t.max)
                ray_t.max = t1;
        } else {
            if (t1 > ray_t.min)
                ray_t.min = t1;
            if (t0 < ray_t.max)
                ray_t.max = t0;
        }

        if (ray_t.max <= ray_t.min)
            return 0;
    }
    return 1;
}

Aabb moveBbox(Aabb aabb, Vector3D offset) {
    aabb.x = shiftInterval(aabb.x, offset.x);
    aabb.y = shiftInterval(aabb.y, offset.y);
    aabb.z = shiftInterval(aabb.z, offset.z);
    return aabb;
}

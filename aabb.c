#include "aabb.h"
#include "interval.h"

Aabb createAabb(Interval x, Interval y, Interval z) {
    Aabb aabb;
    aabb.x = x;
    aabb.y = y;
    aabb.z = z;
    return aabb;
}

Aabb createAabbFromPoints(Point3D a, Point3D b) {
    Aabb aabb;
    aabb.x = (a.x <= b.x) ? createInterval(a.x, b.x) : createInterval(b.x, a.x);
    aabb.y = (a.y <= b.y) ? createInterval(a.y, b.y) : createInterval(b.y, a.y);
    aabb.z = (a.z <= b.z) ? createInterval(a.z, b.z) : createInterval(b.z, a.z);
    return aabb;
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

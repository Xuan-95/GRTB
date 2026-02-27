#ifndef SPHERE_H
#define SPHERE_H

#include "common.h"
#include "hittable.h"

typedef struct {
    Hittable base;
    Point3D center;
    double radius;
} Sphere;

Hittable *createSphere(Point3D center, double radius);
int hitSphere(Hittable *self, Ray *r, Interval ray_t, HitRecord *rec);

#endif
